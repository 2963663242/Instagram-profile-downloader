from downloader import instagramDownloader, get_post

# username = "kingjames"
# loader = instagramDownloader(username)
# profile = loader.get_profile()
# print(profile)
# story = loader.get_story()
# print(story)

url = "http://www.instagram.com/p/CfM3SQDue6Y/?utm_source=ig_web_copy_link"
node = get_post(url)
print(node)

