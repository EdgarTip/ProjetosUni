# systemctl disable firewalld
systemctl stop firewalld
# systemctl enable iptables
#systemctl start iptables

iptables -F -t nat
iptables -F 


	#Drop all communications entering
	iptables -P INPUT DROP

	#DNS name resolution requests sent to outside servers
	iptables -A INPUT -p udp --sport domain -j ACCEPT 
	iptables -A INPUT -p tcp --sport domain -j ACCEPT


	#SSH connections to the router system, if originated at the
        #internal network or at the VPN gateway (vpn-gw)
	iptables -A INPUT -s 192.168.10.0/24 -p tcp --dport ssh -j ACCEPT
	iptables -A INPUT -s 23.214.219.253 -p tcp --dport ssh -j ACCEPT
	iptables -A INPUT -s 192.168.10.0/24 -p udp --dport ssh -j ACCEPT
        iptables -A INPUT -s 23.214.219.253 -p udp --dport ssh -j ACCEPT

	#Drop all communications between networks
	iptables -P FORWARD DROP
	iptables -A FORWARD -j NFQUEUE --queue-num 0

	#Domain name resolutions using the dns server.
	iptables -A FORWARD -p udp -d 23.214.219.253 --dport domain -j ACCEPT	
        iptables -A FORWARD -p tcp -d 23.214.219.253 --dport domain -j ACCEPT


	#The dns server should be able to resolve names using DNS servers on the Internet (dns2 and also others).
	iptables -A FORWARD -p udp -s 23.214.219.253 -d 87.0.0.0/8 --dport domain -j ACCEPT
    	iptables -A FORWARD -p udp -d 23.214.219.253 -s 87.0.0.0/8 --dport domain -j ACCEPT


	#The dns and dns2 servers should be able to synchronize the contents of DNS zones.

	iptables -A FORWARD -p tcp -s 23.214.219.128/25 -d 87.0.0.0/8 --dport domain -j ACCEPT
        iptables -A FORWARD -p tcp -d 23.214.219.128/25 -s 87.0.0.0/8 --dport domain -j ACCEPT

	
	#SMTP connections to the smtp server.
	iptables -A FORWARD -p tcp -d 23.214.219.253 --dport smtp -j ACCEPT	
	iptables -A FORWARD -p udp -d 23.214.219.253 --dport smtp -j ACCEPT

	#POP and IMAP connections to the mail server.
	iptables -A FORWARD -p tcp -d 23.214.219.253 --dport pop3 -j ACCEPT
	iptables -A FORWARD -p tcp -d 23.214.219.253 --dport imap -j ACCEPT
	iptables -A FORWARD -p udp -d 23.214.219.253 --dport pop3 -j ACCEPT
        iptables -A FORWARD -p udp -d 23.214.219.253 --dport imap -j ACCEPT


	#HTTP and HTTPS connections to the www server.
	iptables -A FORWARD -p tcp -d 23.214.219.253 --dport http -j ACCEPT
	iptables -A FORWARD -p tcp -d 23.214.219.253 --dport https -j ACCEPT
	iptables -A FORWARD -p udp -d 23.214.219.253 --dport http -j ACCEPT
        iptables -A FORWARD -p udp -d 23.214.219.253 --dport https -j ACCEPT

	#OpenVPN connections to the vpn-gw server.
	iptables -A FORWARD -p udp -d 23.214.219.253 --dport openvpn -j ACCEPT
	iptables -A FORWARD -p tcp -d 23.214.219.253 --dport openvpn -j ACCEPT

	#VPN clients connected to the gateway (vpn-gw) should be able
	# to connect to all services in the Internal network (assume
	# the gateway does SNAT/MASQUERADING for communications received from clients)
	
	iptables -A FORWARD -p udp -d 192.168.10.0/24 -s 23.214.219.253 --sport openvpn -j ACCEPT
	iptables -A FORWARD -p tcp -d 192.168.10.0/24 -s 23.214.219.253 --sport openvpn -j ACCEPT

	iptables -A FORWARD -m state --state RELATED,ESTABLISHED -j ACCEPT

#Firewall configuration for connections to the external IP address 
#of the firewall (using NAT)

#The connections originated on the outside (Internet) and destined to the external IP address of the firewall should be
#authorized and treated according to the following requirements

modprobe ip_conntrack_ftp
modprobe ip_nat_ftp
modprobe nfnetlink_queue


iptables -A FORWARD -p tcp -s 87.0.0.0/8  --dport ftp -j ACCEPT
iptables -A FORWARD -p tcp -s 192.128.10.253 --sport ftp-data -j ACCEPT

iptables -t nat -A PREROUTING -s 87.0.0.0/8 -d 87.248.214.97 -p tcp --dport ftp -j DNAT --to-destination 192.168.10.253  

iptables -t nat -A POSTROUTING -s 192.168.10.253 -p tcp --sport ftp-data -j SNAT --to-source 87.248.214.97


iptables -A FORWARD -s 192.168.10.253 -p tcp --dport ftp -j ACCEPT
iptables -A FORWARD -s 87.0.0.0/8 -p tcp --sport ftp-data -j ACCEPT


iptables -t nat -A POSTROUTING -s 192.168.10.253 -p tcp --dport ftp -j SNAT --to-source 87.248.214.97
iptables -t nat -A PREROUTING -s 87.0.0.0/8 -p tcp --sport ftp-data -j DNAT --to-destination 192.168.10.253

#SSH connections to the datastore server, but only if originated at the eden or dns2 servers
iptables -A FORWARD -p tcp -s 87.248.214.253 -d 192.168.10.253 --dport ssh -j ACCEPT
iptables -A FORWARD -p tcp -s 87.248.214.252 -d	192.168.10.253 --dport ssh -j ACCEPT

iptables -t nat -A PREROUTING -p tcp -s 87.248.214.253 -d 87.248.214.254 --dport ssh -j DNAT --to-destination 192.168.10.253
iptables -t nat -A PREROUTING -p tcp -s 87.248.214.252 -d 87.248.214.254 --dport ssh -j DNAT --to-destination 192.168.10.253

#Firewall configuration for communications from the internal network to the outside (using NAT)
#Domain name resolutions using DNS
iptables -A FORWARD -p udp -s 192.168.10.0/24 -d 87.0.0.0/8 --dport domain -j ACCEPT
iptables -t nat -A POSTROUTING -p udp -s 192.168.10.0/24 -d 87.0.0.0/8 --dport domain -j SNAT --to-source 87.248.214.97 

iptables -A FORWARD -p tcp -s 192.168.10.0/24 -d 87.0.0.0/8 --dport domain -j ACCEPT
iptables -t nat -A POSTROUTING -p tcp -s 192.168.10.0/24 -d 87.0.0.0/8 --dport domain -j SNAT --to-source 87.248.214.97

#HTTP, HTTPS and SSH connections
iptables -A FORWARD -p tcp -s 192.168.10.0/24 -d 87.248.214.253  --dport http -j ACCEPT 
iptables -A FORWARD -p tcp -s 192.168.10.0/24 -d 87.248.214.253  --dport https -j ACCEPT	
iptables -A FORWARD -p tcp -s 192.168.10.0/24 -d 87.248.214.253  --dport ssh -j ACCEPT

iptables -t nat -A POSTROUTING -p udp -s 192.168.10.0/24 -d 87.0.0.0/8 --dport http -j SNAT --to-source 87.248.214.97
iptables -t nat -A POSTROUTING -p udp -s 192.168.10.0/24 -d 87.0.0.0/8 --dport https -j SNAT --to-source 87.248.214.97
iptables -t nat -A POSTROUTING -p udp -s 192.168.10.0/24 -d 87.0.0.0/8 --dport ssh -j SNAT --to-source 87.248.214.97

iptables -A FORWARD -p tcp -s 87.0.0.0/8 -d 192.168.10.0/24 --dport postgres -j ACCEPT


#FTP connections (in passive and active modes) to external FTP servers

#iptables -A FORWARD -p tcp -s 192.168.10.253 -d 87.0.0.0/8 --dport ftp -j ACCEPT
#iptables -A FORWARD -p tcp -s 87.0.0.0/8 -d 192.168.10.253 --sport ftp-data -j ACCEPT

#iptables -t nat -A POSTROUTING -p tcp -s 192.168.10.253 -d 87.0.0.0/8 --dport ftp -j SNAT --to-source 87.248.214.97

#iptables -t nat -A PREROUTING -p tcp -s 87.0.0.0/8 -d 87.248.214.97 --sport ftp-data -j DNAT --to-destination 192.168.10.253




#Snort 
#iptables -A FORWARD -p tcp --dport 1433 -j ACCEPT
#iptables -A FORWARD -j NFQUEUE --queue-num 0 
iptables -A FORWARD -p tcp --dport postgres -j ACCEPT
iptables -A FORWARD -p icmp -j ACCEPT
