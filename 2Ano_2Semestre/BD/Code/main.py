import jwt
from functools import wraps
from werkzeug.security import generate_password_hash, check_password_hash
import traceback
from flask import Flask, json, request, jsonify
import psycopg2
import time
import threading
import datetime

app = Flask(__name__)
app.config['SECRET_KEY'] = 'thisissecret'  # chave secreta para token

'''
Classe que contém os métodos e objetos necessários para comunicar com a base de dados
'''


class DataBase:
    def __init__(self):
        self.conn2 = psycopg2.connect(user="Pedro",
                                      password="password",
                                      host="localhost",
                                      database="Projeto")

        self.conn2.autocommit = False
        self.cur2 = self.conn2.cursor()

    def execute(self, query, values):
        return self.cur2.execute(query, values)

    def rollback(self):
        self.conn2.rollback()

    def coursor(self):
        return self.cur2

    def fetchone(self):
        return self.cur2.fetchone()

    def fetchall(self):
        return self.cur2.fetchall()

    def commit(self):
        self.conn2.commit()

    def rowcount(self):
        return self.cur2.rowcount

    def close(self):
        self.conn2.close()
        self.cur2.close()


'''
Faz decode do token. O token tem de ser passado por header, sendo que este método verifica a validade do token
'''


def token_required(f):  # f is the function that has the @token_required
    @wraps(f)
    def decorated(*args, **kwargs):

        if 'user-token' in request.headers and request.headers['user-token']:
            token = request.headers['user-token']
        else:
            return jsonify({'erro': 'Token is missing!'})

        query = "select * from utilizador where username = %s"

        db = DataBase()

        try:
            token_decoded = jwt.decode(token, app.config['SECRET_KEY'], algorithms="HS256")
            username = token_decoded['username']
            db.execute(query, (username,))  # VAI BUSCAR O USER COM O USERNAME QUE ESTA NO TOKEN (DESENCRIPTADO, AGR)
            current_user = db.fetchone()
        except:
            db.rollback()
            db.close()
            return jsonify({'erro': 'Token is invalid!'})
        db.commit()
        db.close()
        return f(current_user, *args, **kwargs)

    return decorated


'''
Termina todos os leilões cuja data ultrapassou o limite imposto. Quando um leilão termina, é determinado um vencedor, caso este exista.
'''


def terminar_leiloes():
    db = DataBase()

    # query = "select leilaoid, maioroferta from leilao where tempofinal <= now() and vencedor is null"
    query = """
        select leilaoid, maioroferta, licitacao.utilizador_username from leilao, licitacao 
        where leilao.tempofinal <= now() and leilao.vencedor is null 
        and licitacao.valor = maioroferta
        and licitacao.leilao_leilaoid = leilao.leilaoid;
    """

    try:
        db.execute(query, None)
    except:
        db.rollback()
        print("Erro a selecionar leiloes a terminar")
        # return jsonify({'erro': traceback.print_exc()})
    toEnd = []

    if db.rowcount() != 0:
        for row in db.coursor():
            if row[1] is not None:
                toEnd.append([row[0], row[1], row[2]])  # leilaoId, maioroferta, username (vencedor)
            # values = (row[0], row[1])

    query = "update leilao set vencedor = %s where leilaoId = %s"
    for leilao in toEnd:
        values = (leilao[2], leilao[0])  # userId, leilaoId
        try:
            db.execute(query, values)
            db.commit()
            print("O leilão " + str(leilao[0]) + " terminou, o vencedor é " + str(leilao[2]) + "!")
        except:
            db.rollback()
            print("Erro a dar update dos vencedores")
            # return jsonify({'erro': traceback.print_exc()})
    db.close()
    return 1


'''
End point que força o término dos leilões. 
requisitos: token
'''


@app.route('/dbproj/terminar', methods=['GET'])
@token_required
def call_terminar_leiloes(current_user):
    if not current_user:
        return jsonify({'erro': 'Not authenticated'})
    terminar_leiloes()
    return jsonify({"Sucesso": "A invocar término de leilões"})


'''
Método executado pela thread. Invoca o término de leilões a cada 30 segundos
'''


def worker():
    while 1:
        terminar_leiloes()
        time.sleep(30)


'''
Apanha erros
'''


@app.errorhandler(404)
def page_not_found(e):
    return jsonify({"erro": "Page not found"})


'''
End point que trata do registo de utilizadores. 
requisitos: username, email, password
output: userId : username
'''


@app.route('/dbproj/user', methods=['POST'])
def registar_utilizador():
    username = request.args.get('username')
    email = request.args.get('email')
    password = request.args.get('password')

    if username == "" or email == "" or password == "":
        return jsonify({"erro": "Preecha todos os parâmetros."})

    db = DataBase()

    hashed_password = generate_password_hash(password, method='sha256')
    query = "insert into utilizador values(%s, %s, %s)"
    values = (username, email, hashed_password)

    try:
        db.execute(query, values)
        db.commit()
        db.close()
        return jsonify({"userId": username})
    except:
        db.rollback()
        db.close()
        return jsonify({'erro': "Username já existe"})


'''
End point que autentica o utilizador. Gera um token único que permite ao utilizador usufruir de todas as funcionalidades do programa.
requisitos: username, pass
output: atuhToken: token

'''


@app.route('/dbproj/user', methods=['PUT'])
def autenticar_utilizador():
    auth = request.authorization
    if not auth or not auth.username or not auth.password:
        return jsonify({'erro': 'Username or password missing.'})

    db = DataBase()
    query = "select * from utilizador where username = %s"
    try:
        db.execute(query, (auth.username,))  # VAI BUSCAR O USER COM O USERNAME FORNECIDO
    except:
        db.rollback()
        db.close()
        return jsonify({"erro": traceback.print_exc()})

    if db.rowcount() == 0:
        db.close()
        return jsonify({'erro': 'User invalid.'})

    user = db.fetchone()
    '''
    Username:   user[0]
    Email:      user[1]
    Password:   user[2]
    '''

    if check_password_hash(user[2], auth.password):
        token = jwt.encode({'username': user[0]}, app.config['SECRET_KEY'], algorithm="HS256")
        return jsonify({'authToken': token})

    db.close()

    return jsonify({'erro': 'User invalid.'})  # Teoricamente nao passa aqui, só para certificar


'''
End point responsavel por criar leilões. Um utilizador precisa de utilizar o seu token para aceder a esta funcionalidade. 
requisitos: token, artigo id, preco minimo, titulo, descricao leilao, tempo final, descricao artigo
output: leilaoID: id
'''


@app.route('/dbproj/leilao', methods=['POST'])
@token_required
def criar_leilao(current_user):
    artigo_id = request.args.get('artigoId')
    preco_minimo = request.args.get('precoMinimo')
    titulo = request.args.get('titulo')
    descricao_leilao = request.args.get('descricaoLeilao')
    tempo_final = request.args.get('tempofinal')
    descricao_artigo = request.args.get('descricaoArtigo')

    if not current_user:
        return jsonify({'erro': 'Not authenticated'})

    if artigo_id == "" or preco_minimo == "" or titulo == "" or descricao_leilao == "" or tempo_final == "" or descricao_artigo == "":
        return jsonify({"erro": "Preecha todos os parâmetros."})

    if datetime.datetime.strptime(tempo_final, '%Y-%m-%d %H:%M:%S.%f') <= datetime.datetime.now():
        return jsonify({"erro": "A data inserida é inválida"})
    db = DataBase()
    # Mete um novo artigo se necessário

    query2 = "insert into artigo values(%s, %s)"

    values = (artigo_id, descricao_artigo)
    try:
        db.execute(query2, values)
    except:
        db.rollback()
        db.close()
        return jsonify({'erro': 'Artigo Id is already taken'})

    query4 = "insert into leilao values(default, %s, %s, %s, %s, %s, %s, %s, %s)"

    values = (preco_minimo, None, titulo, descricao_leilao, tempo_final, None, artigo_id, current_user[0])

    try:
        db.execute(query4, values)

        query5 = "select MAX(leilaoid) from leilao"
        db.execute(query5, None)

        id = db.fetchone()[0]
        db.commit()
        db.close()
        return jsonify({'leilaoID': id})
    except:
        db.rollback()
        db.close()
        return jsonify({'erro': traceback.print_exc()})


'''
Este endpoint é responsável por listar todos os leilões que estão a decorrer no momento.
requisitos: token
output: (leilões)
'''


@app.route('/dbproj/leiloes', methods=['GET'])
@token_required
def listar_leiloes(current_user):
    if not current_user:
        return jsonify({'erro': 'Not authenticated'})

    db = DataBase()

    query = "select leilaoid, descricao from leilao where tempofinal > now() order by leilaoid"

    try:

        db.execute(query, None)
        response = []

        for row in db.coursor():
            response.append({'leilaoId': row[0], 'descricao': row[1]})
        db.commit()
        db.close()
        return jsonify(response)

    except Exception as e:
        db.rollback()
        db.close()
        return jsonify({'erro': e})


'''
End point responsável por pesquisar por leilões a partir de uma keyword dada por um utilizador. Devolve todos os leilões que ainda estejam a 
decorrer cujo artigo tenha o id keyword ou tenha a palavra keyword na descricao.
requisitos: token, keyword
output: (leiloes)
'''


@app.route('/dbproj/leiloes/<keyword>', methods=['GET'])
@token_required
def pesquisar_leiloes(current_user, keyword):
    if not current_user:
        return jsonify({'erro': 'Not authenticated'})

    if keyword.isdigit():
        # query = "select leilaoid, descricao from leilao where (artigo_artigoid = %s or lower(descricao) like %s) and tempofinal>now()"
        query = """
        select leilao.leilaoid, leilao.descricao from leilao, artigo 
        where 
        artigo.artigoid = leilao.artigo_artigoid and (artigo.artigoid = %s or lower(artigo.descricao) like %s)
        and tempofinal>now() 
        """
        values = (keyword, '%' + keyword.lower() + '%')
    else:
        # query = "select leilaoid, descricao from leilao where lower(descricao) like %s and tempofinal>now()"
        query = """
               select leilao.leilaoid, leilao.descricao from leilao, artigo 
               where artigo.artigoid = leilao.artigo_artigoid and lower(artigo.descricao) like %s
               and tempofinal>now() 
               """
        values = ('%' + keyword.lower() + '%',)

    db = DataBase()

    response = []

    try:
        db.execute(query, values)
        for row in db.coursor():
            response.append({'leilaoId': row[0], 'descricao': row[1]})
        db.commit()
        db.close()
        return jsonify(response)
    except:
        db.rollback()
        db.close()
        return jsonify({'erro': 'Could not find auctions'})


'''
End point responsável por mostrar todos os detalhes de um leilao dado como parametro. 
requisitos: token, leilaoid
output: (leilao)
'''


@app.route('/dbproj/leilao/<leilaoId>', methods=['GET'])
@token_required
def consultar_leilao(current_user, leilaoId):
    if not current_user:
        return jsonify({'erro': 'Not authenticated'})

    if not leilaoId.isdigit():
        return jsonify({'erro': 'leilaoId inválido'})

    response = []
    query = "select leilaoid, descricao, tempofinal from leilao where leilaoid = %s"
    values = (leilaoId,)

    db = DataBase()
    try:
        db.execute(query, values)
        if db.rowcount() == 0:
            return jsonify({'erro': 'Nenhum leilão correspondente'})

        leilao = db.fetchone()
        response.append({'leilaoId': leilao[0], 'descricao': leilao[1], 'término': leilao[2]})

        response.append("---------------------- Mural ---------------------")

        query = "select mensagem.data, utilizador_username, texto from mensagem where leilao_leilaoid = %s order by mensagem.data asc"
        db.execute(query, values)
        if db.rowcount != 0:
            for row in db.coursor():
                response.append({"Data": row[0], "Utilizador": row[1], "Mensagem": row[2]})  # data, id, mensagem

        response.append("------------------- Licitações -------------------")
        query = "select licitacao.data, utilizador_username, valor from licitacao where leilao_leilaoid = %s order by licitacao.data asc"
        db.execute(query, values)
        if db.rowcount() != 0:
            for row in db.coursor():
                response.append({"Data": row[0], "Utilizador": row[1], "Valor": row[2]})  # data, id, mensagem

    except Exception as e:
        db.rollback()
        db.close()
        return jsonify({'erro': e})

    db.commit()
    db.close()
    return jsonify(response)


'''
End point responsável por listar todos os leilões onde um dado utilizador contenha atividade (Vendedor, mensagem, licitacao). 
requisitos: token
output: (leilões)
'''


@app.route('/dbproj/user/leiloes', methods=['GET'])
@token_required
def listar_leiloes_atividade(current_user):
    if not current_user:
        return jsonify({'erro': 'Not authenticated'})

    db = DataBase()

    query = "SELECT * from licitacao"


    has_licitation = True

    try:
        db.execute(query, None)
    except:
        db.rollback()
        db.close()
        return jsonify({'erro': 'Something went wrong 1!'})

    if db.rowcount() == 0:
        has_licitation = False

    if has_licitation:
        query = """
            SELECT leilaoid, maioroferta, titulo, descricao, tempofinal
            FROM leilao, licitacao
            WHERE ((leilao.leilaoid = licitacao.leilao_leilaoid) and  (licitacao.utilizador_username = %s)) or leilao.utilizador_username = %s
            GROUP BY leilaoid
        """
        values = (current_user[0], current_user[0])

        try:

            db.execute(query, values)
            response = []
            for row in db.coursor():
                leilaoDict = {}
                leilaoDict['leilaoId'] = row[0]
                leilaoDict['preco atual'] = row[1]
                leilaoDict['titulo'] = row[2]
                leilaoDict['descricao'] = row[3]
                leilaoDict['tempo final'] = row[4]
                response.append(leilaoDict)

            db.commit()
            db.close()
            return jsonify(response)

        except:
            db.rollback()
            db.close()
            return jsonify({'erro': 'Something went wrong 2!'})

    else:
        query = """
            SELECT leilaoid, maioroferta, titulo, descricao, tempofinal
            FROM leilao
            WHERE leilao.utilizador_username = %s
            GROUP BY leilaoid
        """
        values = (current_user[0],)

        try:

            db.execute(query, values)
            response = []
            for row in db.coursor():
                leilaoDict = {}
                leilaoDict['leilaoId'] = row[0]
                leilaoDict['preco atual'] = row[1]
                leilaoDict['titulo'] = row[2]
                leilaoDict['descricao'] = row[3]
                leilaoDict['tempo final'] = row[4]
                response.append(leilaoDict)

            db.commit()
            db.close()
            return jsonify(response)

        except:
            db.rollback()
            db.close()
            return jsonify({'erro': 'Something went wrong 3!'})


'''
End point responsavel por criar licitacoes num leilao.
requisitos: token, leilaoid, licitacao
output: Sucesso: Licitacao de (licitacao) no leilao (leilaoid)
'''


@app.route('/dbproj/licitar/<leilaoId>/<licitacao>', methods=['GET'])
@token_required
def efetuar_licitacao(current_user, leilaoId, licitacao):
    if not current_user:
        return jsonify({'erro': 'Not authenticated'})
    if not leilaoId.isdigit():
        return jsonify({'erro': 'leilaoId invalid'})
    if not licitacao.isdigit():
        return jsonify({'erro': 'Valor de licitação inválido'})

    query = "select maioroferta, precominimo from leilao where leilaoid = %s and tempofinal > now()"
    values = (leilaoId,)
    db = DataBase()
    db.execute(query, values)

    if db.rowcount() == 0:
        return jsonify({'erro': 'Leilão não existe ou já terminou'})
    else:
        leilao = db.fetchone()
        if leilao[0] and int(licitacao) <= leilao[0]:  # licitacao é menor do que a maior oferta
            return jsonify({'erro': 'Valor de licitação não é superior à maior oferta'})
        elif int(licitacao) < leilao[1]:  # licitacao é menor do que o valor base
            return jsonify({'erro': 'Valor de licitação inferior ao valor base'})

    query = "insert into licitacao values(%s, default, now(), %s, %s)"  # valor, id, data, username, leilao_id
    values = (licitacao, current_user[0], leilaoId)
    try:
        db.execute(query, values)
    except:
        db.rollback()
        db.close()
        return jsonify({"erro": traceback.print_exc()})

    query = "update leilao set maioroferta = %s where leilaoid = %s"  # update maior oferta do leilao
    values = (licitacao, leilaoId)

    try:
        db.execute(query, values)
    except:
        db.rollback()
        db.close()
        return jsonify({"erro": traceback.print_exc()})

    db.commit()
    db.close()
    return jsonify({"Sucesso": "Licitação de " + licitacao + " no leilão " + leilaoId})


'''
End point responsável por editar a descricao e titulo de um leilao. 
requisitos: token, leilaoid
output: 'leilaoId': leilaoId, 'descricao': descricao, 'titulo': titulo
'''


@app.route('/dbproj/leilao/<leilaoId>', methods=['PUT'])
@token_required
def editar_leilao(current_user, leilaoId):
    titulo = request.args.get('titulo')
    descricao = request.args.get('descricao')

    if not current_user:
        return jsonify({'erro': 'Not authenticated'})
    if titulo == "" and descricao == "":
        return jsonify({"erro": "Alterações não preenchidas."})

    query = """
    select utilizador_username
    from leilao
    where leilaoid = %s and tempofinal > now()
    """

    values = (leilaoId,)

    db = DataBase()

    try:
        db.execute(query, values)
    except:
        db.rollback()
        db.close()
        return jsonify({"erro": traceback.print_exc()})

    if db.rowcount() == 0:
        return jsonify({"erro": "Leilao nao existe ou já terminou"})

    user_name = db.fetchone()

    if current_user[0] != user_name[0]:
        return jsonify({"erro": "Não tem permissao para editar esse leilão"})

    query = """UPDATE leilao
    SET descricao = %s, titulo = %s 
    WHERE leilaoid = %s;
    """

    values = (descricao, titulo, leilaoId)

    try:
        db.execute(query, values)
        db.commit()
        db.close()
        dictio = {'leilaoId': leilaoId, 'descricao': descricao, 'titulo': titulo}
        return jsonify(dictio)

    except:
        db.rollback()
        db.close()
        return jsonify({'erro': 'Id do leilao inválido'})


'''
End point responsavel por entregar a um utilizador todas as notificações nao lidas. Após utilizar este end point, as mensagens passaram a 
estar lidas, nao sendo mais disponíveis.
requisitos: token
output: (notificacoes)
'''


@app.route('/dbproj/user/caixa', methods=['GET'])
@token_required
def caixa_entrada(current_user):
    if not current_user:
        return jsonify({'erro': 'Not authenticated'})

    response = []
    query = """ select notificacao_data, notificacao_texto from notificacao_licitacao 
                where utilizador_username = %s and notificacao_visto = false
                UNION
                select notificacao_data, notificacao_texto from notificacao_mensagem 
                where utilizador_username = %s and notificacao_visto = false 
                order by notificacao_data asc
                """
    values = (current_user[0], current_user[0])

    db = DataBase()
    try:
        db.execute(query, values)
    except:
        db.rollback()
        db.close()
        return jsonify({"erro": traceback.print_exc()})

    if db.rowcount() != 0:
        for row in db.coursor():
            response.append({str(row[0]): row[1]})  # data = texto

    query_update1 = "update notificacao_licitacao set notificacao_visto = true where utilizador_username = %s"
    query_update2 = "update notificacao_mensagem set notificacao_visto = true where utilizador_username = %s"

    values = (current_user[0],)
    try:
        db.execute(query_update1, values)
        db.execute(query_update2, values)
        db.commit()
        db.close()
    except:
        db.rollback()
        db.close()
        return jsonify({"erro": traceback.print_exc()})

    return jsonify(response)


'''
End point responsável pela escrita de mensagens no mural de um leilao. 
requisitos: token, mensagem
output: Sucesso: Mensagem (message) publicadao no mural do leilao leilaoId

'''


@app.route('/dbproj/mural/<leilaoId>', methods=['PUT'])
@token_required
def escrever_mensagem(current_user, leilaoId):
    if not current_user:
        return jsonify({'erro': 'Not authenticated'})

    message = request.args.get('mensagem')
    if message == "":
        return jsonify({"erro": "Escreva a mensagem que pretende publicar."})

    query = "select * from leilao where leilaoid = %s and tempofinal > now() and vencedor is null"
    values = (leilaoId,)

    db = DataBase()

    try:
        db.execute(query, values)

    except:
        db.rollback()
        db.close()
        return jsonify({'erro': traceback.print_exc()})

    if db.rowcount() == 0:
        return jsonify({'erro': 'Leilão não existe ou já terminou'})

    query = "insert into mensagem values(%s, default , now(), %s, %s)"  # texto, id, data, leilaoId, username
    values = (message, leilaoId, current_user[0])

    try:
        db.execute(query, values)
    except:
        db.rollback()
        db.close()
        return jsonify({"erro": traceback.print_exc()})
    db.commit()
    db.close()
    return jsonify({"Sucesso": "Mensagem '" + str(message) + "' publicadao no mural do leilao " + leilaoId})


'''
Funcao main. Cria a thread e corre a aplicacao
'''
if __name__ == '__main__':
    thread = threading.Thread(target=worker)
    thread.daemon = True
    thread.start()
    app.run(debug=False, port=8080)
