import re
import urllib.parse
import http.client

from utils import getSystemProxies

username = "kingjames"
url = "https://www.pixwox.com/profile/"+username


proxies = getSystemProxies()
headers = {'Host': 'www.pixwox.com', 'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/95.0.4638.74 Safari/537.36', 'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8', 'Accept-Language': 'en-us,en;q=0.5', 'Sec-Fetch-Mode': 'navigate', 'Connection': 'close'}

url = urllib.parse.urlparse(url)
if proxies=={}:
  conn = http.client.HTTPSConnection(url.hostname)
else:
  hostname,port = proxies["http"].split(":")
  conn = http.client.HTTPSConnection(hostname,port)
  conn.set_tunnel(url.hostname)

conn.request("GET",url.path,headers=headers)
response = conn.getresponse()

text = response.read().decode("utf-8")
pic_link = re.search('<div class="pic">\n<a href="(.+?)"',text).group(1)
fullname = re.search('<h1 class="fullname">(.+?)</h1>', text).group(1)
posts = re.search("""<div class="item item_posts">
<div class="txt">Posts</div>
<div class="num" title="(.+?)">
.+? </div>""", text).group(1)
Followers = re.search("""<div class="item item_followers">
<div class="txt">Followers</div>
<div class="num" title="(.+?)">
.+? </div>""", text).group(1)
Following = re.search("""<div class="item item_following">
<div class="txt">Following</div>
<div class="num" title="(.+?)">
.+? </div>""", text).group(1)
print("username : "+username)
print("pic_link : " + pic_link)
print("fullname : " + fullname)
print("posts : " + posts)
print("Followers : " + Followers)
print("Following : " + Following)

