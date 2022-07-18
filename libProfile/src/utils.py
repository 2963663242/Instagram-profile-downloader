import re,sys,os,json
from enum import Enum
from functools import wraps
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
    filename = os.path.abspath(filename)
    directory = os.path.dirname(filename)
    if not os.path.exists(directory):
        os.mkdir(directory)
    ret= requests.get(url,proxies=getSystemProxies())
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

def error_process(func):
    @wraps(func)
    def wrapped_function(*args, **kwargs):
        try:
            data =  func(*args, **kwargs)
            ret = {'type':MsgType.finished.value,'msg':{
                'ret_code':'0'
            }}
            ret['msg'].update(data)
        except Exception as e:
            ret = {'type': MsgType.finished.value, 'msg': {
                'ret_code': '-1',
                'exception':str(e.args[0])
            }}
        return ret
    return wrapped_function