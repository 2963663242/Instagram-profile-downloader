import re,sys,os,json,time
from enum import Enum
from functools import wraps
from urllib.request import getproxies

import func_timeout
import requests
from func_timeout import func_set_timeout

from inslog import logger


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


def downpic(url,filename):
    try:
        downpic_internal(url,filename)
    except func_timeout.exceptions.FunctionTimedOut as e:
        logger.error("download img Failed ,download Timeout")
        raise Exception("image download timeout, the reson ：", e)
@func_set_timeout(10)
def downpic_internal(url,filename):
    logger.info("download img start, filename == %s, url== %s" % (filename,url))
    filename = os.path.abspath(filename)
    directory = os.path.dirname(filename)
    if not os.path.exists(directory):
        logger.warning("directory  %s not existed,now to crate it " % (directory))
        os.mkdir(directory)
    ret= requests.get(url,proxies=getSystemProxies())
    if ret.status_code == 200:
        try:
            with open(filename, 'wb') as fp:
                fp.write(ret.content)
            logger.info("download img successful")
        except Exception as e:
            logger.error("save img to local Failed")
            raise e
    else:
        logger.error("download img Failed, http code: %s" % ret.status_code)
    logger.info("download img end")
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


def pack_post(data):
    node = {
        'display_url': data['display_url'],
        'height': data['dimensions']['height'],
        'width': data['dimensions']['width']
    }
    if (data['is_video'] == True):
        node['video_url'] = data['video_url']
    return node
def pack_story(item):
    image = item['image_versions2']['candidates'][0]
    node = {
        'display_url': image['url'],
        'height': image['height'],
        'width': image['width']
    }
    if item['media_type'] == 2:
        node['video_url'] = item['video_versions'][0]['url']
        node['height'] = item['video_versions'][0]['height']
        node['width'] = item['video_versions'][0]['width']
    return node
def is_json(file):
    try:
        json.loads(file)
    except Exception:
        return False
    return True