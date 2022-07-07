import http.client
import json
import ssl
import urllib.parse
from functools import wraps
from urllib.parse import urljoin
from urllib.parse import urlparse
from urllib.parse import urlunparse
from posixpath import normpath

import browser_cookie3
import requests

from utils import getSystemProxies
import re
ctx = ssl.create_default_context()
ctx.check_hostname = False
ctx.verify_mode = ssl.CERT_NONE

def error_process(func):
    @wraps(func)
    def wrapped_function(*args, **kwargs):
        try:
            return func(*args, **kwargs)
        except Exception as e:
            pass
        return None

    return wrapped_function

class instagramDownloader():
    def __init__(self,username):
        self.username = username
        self.data = self.fetch_matadata()
    def fetch_matadata(self):
        url = "https://i.instagram.com/api/v1/users/web_profile_info/?username=" + self.username

        payload = {}
        headers = {
            'Host': 'i.instagram.com',
            'User-Agent': "Instagram 146.0.0.27.125 (iPhone12,1; iOS 13_3; en_US; en-US; scale=2.00; 1656x3584; 190542906)"
        }

        response = requests.request("GET", url, headers=headers, data=payload, proxies=getSystemProxies())

        data = json.loads(response.text)
        return data

    @error_process
    def get_profile(self):

        user = self.data["data"]["user"]
        full_name = user["full_name"]
        profile_pic_url = user['profile_pic_url_hd']
        edge_owner_to_timeline_media = user['edge_owner_to_timeline_media']["count"]
        edge_follow = user['edge_follow']["count"]
        edge_followed_by = user['edge_followed_by']["count"]

        return {"username":self.username,"full_name":full_name,"avatar_url":profile_pic_url,"post":edge_owner_to_timeline_media,"Followers":edge_follow,"Following":edge_followed_by}

    @error_process
    def get_story(self):

        userid = self.data["data"]["user"]["id"]
        url = "https://i.instagram.com/api/v1/feed/user/" + userid + "/story/"

        payload = {}
        headers = {
            'Host': 'i.instagram.com',
            'User-Agent': "Instagram 146.0.0.27.125 (iPhone12,1; iOS 13_3; en_US; en-US; scale=2.00; 1656x3584; 190542906)"
        }
        cj = browser_cookie3.chrome(domain_name="instagram.com")

        response = requests.request("GET", url, headers=headers, data=payload,cookies=cj,proxies=getSystemProxies())
        data = response.json()
        items = data['reel']['items']
        nodes  = []
        for item in items:
            image = item['image_versions2']['candidates'][0]
            node = {
                    'display_url': image['url'],
                    'height': image['height'],
                    'width': image['width']
                }
            if item['media_type'] == 2:
                node['video_url'] = item['video_versions'][0]['url']
            nodes.append(node)
        return nodes
def pack_node(data):
    node = {
        'display_url': data['display_url'],
        'height': data['dimensions']['height'],
        'width': data['dimensions']['width']
    }
    if (data['is_video'] == True):
        node['video_url'] = data['video_url']
    return node

@error_process
def get_post(url):

    urlparsed = urllib.parse.urlparse(url)
    if not urlparsed.path.endswith("embed") and not urlparsed.path.endswith("embed/"):
         url = urlunparse((urlparsed.scheme, urlparsed.netloc, urljoin(urlparsed.path,"embed",True), urlparsed.params, urlparsed.query, urlparsed.fragment))

    payload = {}
    headers = {
        #'Host': 'www.instagram.com',
        'User-Agent': "Instagram 146.0.0.27.125 (iPhone12,1; iOS 13_3; en_US; en-US; scale=2.00; 1656x3584; 190542906)"
    }

    response = requests.request("GET", url, headers=headers, data=payload, proxies=getSystemProxies())
    text = response.text

    data = re.search("window.__additionalDataLoaded\('extra',(.+?)\);</script>",text).group(1)
    data = json.loads(data)['shortcode_media']
    node = pack_node(data)
    if 'edge_sidecar_to_children' in data:
        sub_nodes = []
        for sub in data['edge_sidecar_to_children']['edges']:
            sub_nodes.append(pack_node(sub["node"]))
        node['children'] =  sub_nodes

    return node

