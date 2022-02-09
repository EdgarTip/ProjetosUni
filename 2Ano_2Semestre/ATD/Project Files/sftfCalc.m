%%
%CODIGO DESENVOLVIDO POR:
%Edgar Filipe Ferreira Duarte
%Pedro Guilherme da Cruz Ferreira
%DEI 2020/2021

function [] = sftfCalc(data)
    fs =50;
    x = detrend(data(:,3)); %Apenas eixo do z
    N=length(x);
    
    deltaT = 1/fs/60 ;
    time = 1*deltaT: deltaT: length(data)*deltaT;

    t = linspace(0,(length(x)-1)/fs,length(x));
    Tframe = 3;
    Toverlap = Tframe/2; 
    Nframe = round(Tframe*fs); % número de amostras na janela
    Noverlap = round(Toverlap*fs); % número de amostras sobrepostas na janela

    janelaHamming = hamming(Nframe); % janela de hamming

    if mod(Nframe, 2)==0
        f_frame = -fs/2:fs/Nframe:fs/2-fs/Nframe;
    else 
        f_frame = -fs/2+fs/(2*Nframe):fs/Nframe:fs/2-fs/(2*Nframe);
    end

    f_relev = [];
    tframes = [];
    nframes = 0; % para guardar
    fprintf('Janelas: %d\n',numel(1:Nframe-Noverlap:N-Nframe+1));
    for ii = 1:Nframe-Noverlap:N-Nframe+1

        % aplicar a janela ao sinal do tempo
        x_frame = x(ii:ii+Nframe-1).*janelaHamming;
        
        [f_frame,m_X_frame] = dftCalc(x_frame,fs);      %aplicar dft à janela - devolve frequencias e respetivas magnitudes

        [~, ind] = max(m_X_frame);             %Máxima magnitude da janela e seu indice no array de magnitudes m_X_frame
 
        f_relev = [f_relev, abs(f_frame(ind))];         % encontrar as frequências correspondentes ao máximo de magnitude

        nframes = nframes+1;
    end

    deltat = Tframe-Toverlap;
    tframes=deltat:deltat:deltat*nframes;
    tframes = tframes./60;
figure()
subplot(1,2,1);
plot(tframes,f_relev,'o')
xlabel('t [s]');
ylabel('f [Hz]');
title('Sequência de frequências por janelas');
hold on
subplot(1,2,2);
spectrogram(x,Nframe,Noverlap,[],fs,'yaxis')

end