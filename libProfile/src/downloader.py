import json,os,ssl,re
import urllib.parse
from urllib.parse import urljoin
from urllib.parse import urlunparse

import browser_cookie3
import requests

from utils import getSystemProxies, downpic, filter_filename, error_process, pack_story, pack_post
from inslog import logger

ctx = ssl.create_default_context()
ctx.check_hostname = False
ctx.verify_mode = ssl.CERT_NONE


class InstagramExtractor():

    def fetch_matadata(self):
        logger.info("fetch_matadata start")
        url = "https://i.instagram.com/api/v1/users/web_profile_info/?username=" + self.username

        payload = {}
        headers = {
            'Host': 'i.instagram.com',
            'User-Agent': "Instagram 146.0.0.27.125 (iPhone12,1; iOS 13_3; en_US; en-US; scale=2.00; 1656x3584; 190542906)"
        }

        response = requests.request("GET", url, headers=headers, data=payload, proxies=getSystemProxies())


        if(response.status_code == 200):
            data = json.loads(response.text)
            if(data["status"] == 'ok'):
                logger.info("fetch_matadata successful")
            else:
                logger.error("fetch_matadata Failed, status: %s" % data["status"])
        else:
            logger.error("fetch_matadata Failed, http code: %s" % response.status_code)

        logger.info("fetch_matadata end")
        return data


    def get_profile(self,username):
        logger.info("get_profile start")
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
        logger.info("get_profile end")
        return data


    def get_story(self,username):
        logger.info("get_story start")
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

        if (response.status_code == 200):
            data = response.json()
            if (data["status"] == 'ok'):
                if(data["reel"] != None):
                    logger.info("get_story successful")
                else:
                    logger.error("the user have no story")
            else:
                logger.error("get_story Failed, status: %s" % data["status"])
        elif response.status_code == 403:
            logger.error("get_story Failed, please log in")
        else:
            logger.error("get_story Failed, http code: %s" % response.status_code)

        items = data['reel']['items']
        nodes  = []
        for item in items:
            node = pack_story(item)
            nodes.append(node)
        logger.info("get_story end")
        return {"stories":nodes}

    def get_post_by_media_id(self,id):
        logger.info("get_post_by_media_id start")
        url = "https://i.instagram.com/api/v1/media/" + id + "/info/"

        payload = {}
        headers = {
            'Host': 'i.instagram.com',
            'User-Agent': "Instagram 146.0.0.27.125 (iPhone12,1; iOS 13_3; en_US; en-US; scale=2.00; 1656x3584; 190542906)"
        }
        cj = browser_cookie3.load(domain_name="instagram.com")

        response = requests.request("GET", url, headers=headers, data=payload, cookies=cj, proxies=getSystemProxies())
        if (response.status_code == 200):
            data = response.json()
            if (data["status"] == 'ok'):
                logger.info("get_post_by_media_id successful")
            else:
                logger.warning("get_post_by_media_id Failed, status: %s" % data["status"])
        elif response.status_code == 403:
            logger.warning("get_post_by_media_id Failed,get post data maybe incomplete, please log in")
        else:
            logger.warning("get_post_by_media_id Failed,get post data maybe incomplete, http code: %s" % response.status_code)
        items = data['items'][0]
        nodes = []
        if items["media_type"] & 0x3 != 0:
            node = pack_story(items)
            nodes.append(node)
        else:
            items = items['carousel_media']
            for item in items:
                node = pack_story(item)
                nodes.append(node)
        logger.info("get_post_by_media_id end")
        return nodes

    def get_post(self,url):
        logger.info("get_post start")
        urlparsed = urllib.parse.urlparse(url)
        if not urlparsed.path.endswith("embed") and not urlparsed.path.endswith("embed/"):
            url = urlunparse((urlparsed.scheme, urlparsed.netloc, urljoin(urlparsed.path, "embed", True),
                              urlparsed.params, urlparsed.query, urlparsed.fragment))
        logger.info("use embed url == %s" % url)
        payload = {}
        headers = {
            'Host': 'www.instagram.com',
            'User-Agent': "Instagram 146.0.0.27.125 (iPhone12,1; iOS 13_3; en_US; en-US; scale=2.00; 1656x3584; 190542906)"
        }

        response = requests.request("GET", url, headers=headers, data=payload, proxies=getSystemProxies())
        text = response.text
        if response.status_code != 200:
            logger.error("get embed post Failed, the status code is %s",response.status_code)
        data = re.search("window.__additionalDataLoaded\('extra',(.+?)\);</script>", text).group(1)
        try:
            id = re.search('data-media-id="(.+?)"', text).group(1)
            node = self.get_post_by_media_id(id)
        except Exception as e:
            if data == 'null':
                logger.warning("not additionalDataLoaded, now try to get post by media id, sure you already log in")
                display_url = re.search('<img class="EmbeddedMediaImage" alt="Instagram post shared by @.+?" src="(.+?)" srcset',text).group(1)
                display_url = display_url.replace("&amp;","&")
                node = [{'display_url':display_url}]
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
        logger.info("get_post end")
        return {"post":node}



class InstagramDownloader():
    def __init__(self,save_path=""):
        self.save_path = save_path

    @error_process
    def download_profile(self,username):
        logger.info("download_profile start")
        extractor = InstagramExtractor()
        data = extractor.get_profile(username=username)
        real_filename = filter_filename(data["username"])
        profile = data["profile"]

        for i,p in enumerate(profile):
            path = os.path.join(self.save_path, real_filename + "_profile_" +str(i)+ ".jpg")
            downpic(p['display_url'],path)
            p['path'] = path
        logger.info("download_profile end")
        return data
    @error_process
    def download_story(self,username):
        logger.info("download_story start")
        extractor = InstagramExtractor()
        data = extractor.get_story(username = username)
        real_filename = filter_filename(username)
        stories = data['stories']
        for i,s in enumerate(stories):
            path = os.path.join(self.save_path, real_filename + "_story_" +str(i)+ ".jpg")
            downpic(s['display_url'],path)
            s['path'] = path
        logger.info("download_story end")
        return data

    @error_process
    def download_post(self, url):
        logger.info("download_post start")
        extractor = InstagramExtractor()
        data = extractor.get_post(url=url)
        filename = urllib.parse.urlparse(url).path.split('/')[2]
        real_filename = filter_filename(filename)
        post = data['post']
        for i, s in enumerate(post):
            path = os.path.join(self.save_path, real_filename + "_post_" + str(i) + ".jpg")
            downpic(s['display_url'], path)
            s['path'] = path
        logger.info("download_post end")
        return data