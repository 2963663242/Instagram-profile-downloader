import json,os,ssl,re
import urllib.parse
from urllib.parse import urljoin
from urllib.parse import urlunparse

import browser_cookie3
import requests

from utils import getSystemProxies, downpic, filter_filename, error_process, pack_story, pack_post

ctx = ssl.create_default_context()
ctx.check_hostname = False
ctx.verify_mode = ssl.CERT_NONE


class InstagramExtractor():

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


    def get_profile(self,username):
        self.username = username
        self.data = self.fetch_matadata()
        user = self.data["data"]["user"]
        full_name = user["full_name"]
        profile_pic_url = user['profile_pic_url']
        profile_pic_url_hd = user['profile_pic_url_hd']
        edge_owner_to_timeline_media = user['edge_owner_to_timeline_media']["count"]
        edge_follow = user['edge_follow']["count"]
        edge_followed_by = user['edge_followed_by']["count"]
        data = {"username":self.username,"full_name":full_name,"profile":[{"display_url":profile_pic_url,'height':150,'width':150},{"display_url":profile_pic_url_hd,'height':320,'width':320}],"post":edge_owner_to_timeline_media,"Followers":edge_follow,"Following":edge_followed_by}
        return data


    def get_story(self,username):
        self.username = username
        self.data = self.fetch_matadata()
        userid = self.data["data"]["user"]["id"]
        url = "https://i.instagram.com/api/v1/feed/user/" + userid + "/story/"

        payload = {}
        headers = {
            'Host': 'i.instagram.com',
            'User-Agent': "Instagram 146.0.0.27.125 (iPhone12,1; iOS 13_3; en_US; en-US; scale=2.00; 1656x3584; 190542906)"
        }
        cj = browser_cookie3.load(domain_name="instagram.com")

        response = requests.request("GET", url, headers=headers, data=payload,cookies=cj,proxies=getSystemProxies())
        data = response.json()
        items = data['reel']['items']
        nodes  = []
        for item in items:
            node = pack_story(item)
            nodes.append(node)
        return {"stories":nodes}

    def get_post_by_media_id(self,id):
        url = "https://i.instagram.com/api/v1/media/" + id + "/info/"

        payload = {}
        headers = {
            'Host': 'i.instagram.com',
            'User-Agent': "Instagram 146.0.0.27.125 (iPhone12,1; iOS 13_3; en_US; en-US; scale=2.00; 1656x3584; 190542906)"
        }
        cj = browser_cookie3.load(domain_name="instagram.com")

        response = requests.request("GET", url, headers=headers, data=payload, cookies=cj, proxies=getSystemProxies())
        data = response.json()
        items = data['items']
        nodes = []
        for item in items:
            node = pack_story(item)
            nodes.append(node)
        return nodes

    def get_post(self,url):
        urlparsed = urllib.parse.urlparse(url)
        if not urlparsed.path.endswith("embed") and not urlparsed.path.endswith("embed/"):
            url = urlunparse((urlparsed.scheme, urlparsed.netloc, urljoin(urlparsed.path, "embed", True),
                              urlparsed.params, urlparsed.query, urlparsed.fragment))

        payload = {}
        headers = {
            'Host': 'www.instagram.com',
            'User-Agent': "Instagram 146.0.0.27.125 (iPhone12,1; iOS 13_3; en_US; en-US; scale=2.00; 1656x3584; 190542906)"
        }

        response = requests.request("GET", url, headers=headers, data=payload, proxies=getSystemProxies())
        text = response.text

        data = re.search("window.__additionalDataLoaded\('extra',(.+?)\);</script>", text).group(1)
        if data == 'null':
            display_url = re.search('<img class="EmbeddedMediaImage" alt="Instagram post shared by @.+?" src="(.+?)" srcset',text).group(1)
            display_url = display_url.replace("&amp;","&")
            node = [{'display_url':display_url}]
            try:
                id = re.search('data-media-id="(.+?)"',text).group(1)
                node = self.get_post_by_media_id(id)
            except Exception as e:
                pass
        else:
            data = json.loads(data)['shortcode_media']
            node = []
            if 'edge_sidecar_to_children' in data:
                sub_nodes = []
                for sub in data['edge_sidecar_to_children']['edges']:
                    sub_nodes.append(pack_post(sub["node"]))
                node = sub_nodes
            else:
                node.append(pack_post(data))

        return {"post":node}



class InstagramDownloader():
    def __init__(self,save_path=""):
        self.save_path = save_path

    @error_process
    def download_profile(self,username):
        extractor = InstagramExtractor()
        data = extractor.get_profile(username=username)
        real_filename = filter_filename(data["username"])
        profile = data["profile"]
        for i,p in enumerate(profile):
            path = os.path.join(self.save_path, real_filename + "_profile_" +str(i)+ ".jpg")
            downpic(p['display_url'],path)
            p['path'] = path
        return data
    @error_process
    def download_story(self,username):
        extractor = InstagramExtractor()
        data = extractor.get_story(username = username)
        real_filename = filter_filename(username)
        stories = data['stories']
        for i,s in enumerate(stories):
            path = os.path.join(self.save_path, real_filename + "_story_" +str(i)+ ".jpg")
            downpic(s['display_url'],path)
            s['path'] = path
        return data

    @error_process
    def download_post(self, url):
        extractor = InstagramExtractor()
        data = extractor.get_post(url=url)
        filename = urllib.parse.urlparse(url).path.split('/')[2]
        real_filename = filter_filename(filename)
        post = data['post']
        for i, s in enumerate(post):
            path = os.path.join(self.save_path, real_filename + "_post_" + str(i) + ".jpg")
            downpic(s['display_url'], path)
            s['path'] = path
        return data