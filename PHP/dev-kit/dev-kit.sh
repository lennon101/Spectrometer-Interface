#!/bin/bash
sudo apt-get update
sudo apt-get install apache2 -y
sudo apt-get install php5 libapache2-mod-php5 -y
sudo apt-get install php-pear
sudo pear install channel://pear.php.net/Config_Lite-0.2.4
sudo ln -s /usr/lib/cgi-bin /var/www/cgi-bin
sudo mkdir /var/www/images
sudo mkdir /var/www/css
sudo mkdir /etc/ocean
sudo apt-get install hostapd -y
sudo apt-get install isc-dhcp-server -y
sudo apt-get install libboost-thread1.50-dev -y
sudo apt-get install libboost-system1.50-dev -y
sudo apt-get install libboost-filesystem1.50-dev -y
sudo apt-get install libboost-chrono1.50-dev -y

sudo chmod a+w /etc/network/interfaces
sudo echo "auto lo" > /etc/network/interfaces
sudo echo "iface lo inet loopback" >> /etc/network/interfaces
sudo echo "iface eth0 inet dhcp" >> /etc/network/interfaces
sudo echo "allow-hotplug wlan0" >> /etc/network/interfaces
sudo echo "iface wlan0 inet static" >> /etc/network/interfaces
sudo echo "  address 192.168.42.1" >> /etc/network/interfaces
sudo echo "  netmask 255.255.255.0" >> /etc/network/interfaces
sudo chmod a-w /etc/network/interfaces

sudo sed -i '/DHCPD_CONF/c\DHCPD_CONF=\/etc\/dhcp\/dhcpd.conf' /etc/default/isc-dhcp-server

sudo touch /etc/dhcp/dhcpd.conf
sudo chmod 666 /etc/dhcp/dhcpd.conf
sudo echo 'INTERFACES="wlan0";' >> /etc/dhcp/dhcpd.conf
sudo echo "subnet 192.168.42.0 netmask 255.255.255.0 {" >> /etc/dhcp/dhcpd.conf
sudo echo "  range 192.168.42.10 192.168.42.20;" >> /etc/dhcp/dhcpd.conf
sudo echo "}" >> /etc/dhcp/dhcpd.conf
sudo chmod 755 /etc/dhcp/dhcpd.conf


sudo unzip hostapd.zip 
sudo mv /usr/sbin/hostapd /usr/sbin/hostapd.bak
sudo mv hostapd /usr/sbin/hostapd.edimax 
sudo ln -sf /usr/sbin/hostapd.edimax /usr/sbin/hostapd 
sudo chown root.root /usr/sbin/hostapd 

sudo sed -i '/DAEMON_CONF/c\DAEMON_CONF=\/etc\/hostapd\/hostapd.conf' /etc/default/hostapd

sudo touch /etc/hostapd/hostapd.conf
sudo chmod 666 /etc/hostapd/hostapd.conf
sudo echo "interface=wlan0" > /etc/hostapd/hostapd.conf
sudo echo "driver=rtl871xdrv" >> /etc/hostapd/hostapd.conf
sudo echo "ssid=ocean-spectrometer" >> /etc/hostapd/hostapd.conf
sudo echo "channel=1" >> /etc/hostapd/hostapd.conf
sudo echo "wmm_enabled=0" >> /etc/hostapd/hostapd.conf
sudo echo "wpa=1" >> /etc/hostapd/hostapd.conf
sudo echo "wpa_passphrase=oceanwifi" >> /etc/hostapd/hostapd.conf
sudo echo "wpa_key_mgmt=WPA-PSK" >> /etc/hostapd/hostapd.conf
sudo echo "wpa_pairwise=TKIP" >> /etc/hostapd/hostapd.conf
sudo echo "rsn_pairwise=CCMP" >> /etc/hostapd/hostapd.conf
sudo echo "auth_algs=1" >> /etc/hostapd/hostapd.conf
sudo echo "macaddr_acl=0 " >> /etc/hostapd/hostapd.conf
sudo chmod 755 /usr/sbin/hostapd

echo -e 'ocean\nocean' | sudo -S passwd
sudo mkdir /home/ocean
sudo useradd -d /home/ocean ocean
sudo chown ocean:ocean /home/ocean
echo -e 'ocean\nocean' | sudo -S passwd ocean
sudo adduser ocean sudo
echo -e 'ocean\nocean' | sudo -S passwd www-data
sudo adduser www-data root
sudo chmod a+w /home/ocean

sudo touch /etc/sudoers.d/apache
sudo chmod 666 /etc/sudoers.d/apache
sudo echo "www-data ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/apache
sudo chmod 440 /etc/sudoers.d/apache

sudo cp libs/libseabreeze-rpi.1.0.so /usr/lib
sudo cp libs/libreqhandler.2.0.so /usr/lib
sudo cp daemon/ocean-daemon.2.0 /usr/sbin
sudo chmod a+x /usr/sbin/ocean-daemon.2.0
sudo cp php/*.php /var/www/cgi-bin
sudo cp raphael.js /var/www/cgi-bin
sudo cp configuration/ocean-daemon.conf /etc/ocean
sudo chmod a+rw /etc/ocean/ocean-daemon.conf
sudo cp oo.gif /var/www/images
sudo cp ocean.css /var/www/css
sudo cp SeaBreeze/os-support/linux/10-oceanoptics.rules /etc/udev/rules.d

sudo ln -s /usr/lib/libseabreeze-rpi.1.0.so /usr/lib/libseabreeze.so
sudo ln -s /usr/lib/libreqhandler.2.0.so /usr/lib/libreqhandler.so
sudo ln -s /usr/sbin/ocean-daemon.2.0 /usr/sbin/ocean-daemon
sudo ln -s /lib/arm-linux-gnueabihf/libusb-0.1.so.4.4.4 /lib/arm-linux-gnueabihf/libusb.so

sudo cp service/ocean /etc/init.d
sudo chmod 755 /etc/init.d/ocean
sudo update-rc.d ocean defaults
