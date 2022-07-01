import browser_cookie3
import requests
from urllib.request import getproxies
import re
import json

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


# cj = browser_cookie3.chrome(domain_name="instagram.com")
username = "kingjames"
# webpage = requests.get(url="https://www.instagram.com/"+username,cookies=cj,proxies=getSystemProxies()).text
#
#
# #url_list = re.findall('\"profile_pic_url\":\"https:.+?\"',webpage.text)
# ID = re.search(r'"X-IG-App-ID":"([0-9]+)"',webpage,re.M|re.I).group(1)

url = "https://i.instagram.com/api/v1/users/web_profile_info/?username="+username

payload={}
headers = {
  'Host':'i.instagram.com',
  'User-Agent': "Instagram 146.0.0.27.125 (iPhone12,1; iOS 13_3; en_US; en-US; scale=2.00; 1656x3584; 190542906)"
}

response = requests.request("GET", url, headers=headers, data=payload,proxies=getSystemProxies())



data = json.loads(response.text)

user = data["data"]["user"]

full_name = user["full_name"]
profile_pic_url = user['profile_pic_url_hd']
edge_owner_to_timeline_media = user['edge_owner_to_timeline_media']["count"]
edge_follow = user['edge_follow']["count"]
edge_followed_by = user['edge_followed_by']["count"]

print("username : "+username)
print("full_name : "+full_name)
print("profile_pic_url : "+profile_pic_url)
print("post : "+str(edge_owner_to_timeline_media))
print("Followers : "+str(edge_followed_by))
print("Following : "+str(edge_follow))
