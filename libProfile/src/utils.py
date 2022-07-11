import re,sys,os
from enum import Enum
from urllib.request import getproxies
import requests
from func_timeout import func_set_timeout

class MsgType(Enum):
    param = 'parsing'
    sniff = 'parsed'
    playlist = 'playlist'
    downloading = 'downloading'
    converting = 'converting'
    converted = 'converted'
    finished = 'finished'
    sleep = 'sleep'
def flush_print(data):
    print(data)
    sys.stdout.flush()

@func_set_timeout(10)
def downpic(url,filename):
    directory = os.path.dirname(filename)
    if not os.path.exists(directory):
        os.mkdir(directory)
    ret= requests.get(url)
    if ret.status_code == 200:
        with open(filename, 'wb') as fp:
            fp.write(ret.content)

def getSystemProxies():
  auto_proxies = getproxies()
  if auto_proxies != {}:
    proxies = {
      'http': auto_proxies['http'].replace("http://", ""),
      'https': auto_proxies['https'].replace("https://", ""),
    }
  else:
    proxies = {}
  return proxies

def filter_filename(filename):
    real_filename = re.sub('[\/:*?"<>|]', '_',filename)
    return real_filename