
echo -e "\n"
echo "|=================================================|"
echo "| 　　openssh 安装脚本 @ TWen　　　　　　　     　|"
echo "|=================================================|"
echo -e "\n"

# 创建目录
echo "[1] 创建目录:/usr/local /var/run /var/empty /etc/shadow "
if [ ! -d /usr/local ]
then
	echo "创建 /usr/local"
	mkdir /usr/local
fi

if [ ! -d /var/run ]
then
	echo "创建 /var/run"
	mkdir /var/run	
fi

if [ ! -d /var/empty ]
then
	echo "创建 /var/empty"
	mkdir /var/empty
fi

if [ ! -d /etc/shadow ]
then
	echo "创建 /etc/shadow"
	mkdir /etc/shadow
fi


# 解压openssh.tar

echo "[2] 解压 openssh.tar"

tar -xvf openssh.tar


if test $? -eq 0
then
	echo "解压成功"
else
	echo "解压失败"
	exit 1
fi

# 拷贝bin,etc,libexec,sbin到/usr/local

echo "[3] 拷贝 openssh/bin openssh/etc openssh/libexec openssh/sbin 到 /usr/local"

cp -r openssh/bin     /usr/local
cp -r openssh/etc     /usr/local
cp -r openssh/libexec /usr/local
cp -r openssh/sbin    /usr/local

# 拷贝libz.so.1 libcrypto.so.1.0.0 到/lib
echo "[4] 拷贝 openssh/libz.so.1 openssh/libcrypto.so.1.0.0 到 /lib"
cp openssh/libz.so.1          /lib
cp openssh/libcrypto.so.1.0.0 /lib

# 添加"PermitRootLogin yes"到/usr/local/etc/sshd_config文件末尾
echo -e "[5] 添加 \"PermitRootLogin yes\" 到 /usr/local/etc/sshd_config　文件末尾"
echo "PermitRootLogin yes" >> /usr/local/etc/sshd_config

# 添加"sshd:x:74:74:Privilege-separated SSH:/var/empty/sshd:/sbin/nologin"　到　/etc/passwd
echo -e "[6] 添加 \"sshd:x:74:74:Privilege-separated SSH:/var/empty/sshd:/sbin/nologin \"to /etc/passwd"
echo "sshd:x:74:74:Privilege-separated SSH:/var/empty/sshd:/sbin/nologin" >>  /etc/passwd

# 添加 "/usr/local/sbin/sshd" 到 /etc/profile
echo -e "[7] 添加 \"/usr/local/sbin/sshd\" to /etc/profile"
echo "/usr/local/sbin/sshd" >> /etc/profile
chmod u+x /usr/local/sbin/sshd

# 添加 "/usr/local/sbin/sshd" 到 /etc/profile
echo -e "[8] 添加 \"export PATH=/usr/local/bin:$PATH\" to /etc/profile"
echo "export PATH=/usr/local/bin:$PATH" >> /etc/profile





#　修改key权限
echo "[9] 修改key权限"
cd /usr/local/etc/
chmod 600 ssh_host_dsa_key
chmod 600 ssh_host_ecdsa_key
chmod 600 ssh_host_ed25519_key
chmod 600 ssh_host_rsa_key
cd -

# 设置root密码
echo -e "[10] 是否已经设置过root密码, y or n?"
read buf
if [ $buf == "n" ]
then
	#sudo -s
	passwd root
	echo -e "\n"
fi


# 显示操作完成
echo "[11] 所有操作完成,请求重启"
echo "重启,y or n ?"
read buf
if [ $buf == "y" ]
then
	echo "３秒后复位 "
	echo "3..."
	sleep 1s
	echo "2.."
	sleep 1s
	echo "1."
	sleep 1s
	echo "reboot"
	sleep 1s
	reboot
fi
