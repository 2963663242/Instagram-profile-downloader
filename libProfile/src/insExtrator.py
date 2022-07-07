import os,sys

from downloader import instagramDownloader, get_post
from argparse import ArgumentParser, ArgumentTypeError, SUPPRESS

def usage_string():
    # NOTE: duplicated in README.rst and docs/index.rst
    argv0 = os.path.basename(sys.argv[0])
    argv0 = "insExtrator" if argv0 == "__main__.py" else argv0
    return """
    download profile 
        usage: {0} profile $username
    download stroy
        usage: {0} story $username
    download posts
        usage: {0} posts $url
    """.format(argv0)

parser = ArgumentParser(description=__doc__, add_help=False, usage=usage_string(),
                            epilog="The complete documentation can be found at "
                                   "https://instaloader.github.io/.",
                            fromfile_prefix_chars='+')
g_targets = parser.add_argument_group("What to Download",
                                          "Specify a list of targets. For each of these, Instaloader creates a folder "
                                          "and downloads all posts. The following targets are supported:")
g_targets.add_argument('profile', nargs='*',
                           help="Download profile. If an already-downloaded profile has been renamed, Instaloader "
                                "automatically finds it by its unique ID and renames the folder likewise.")
g_targets.add_argument('stroy', nargs='*',
                           help="Download profile. If an already-downloaded profile has been renamed, Instaloader "
                                "automatically finds it by its unique ID and renames the folder likewise.")
g_targets.add_argument('posts', nargs='*',
                           help="Download profile. If an already-downloaded profile has been renamed, Instaloader "
                                "automatically finds it by its unique ID and renames the folder likewise.")
args = parser.parse_args()
# username = "kingjames"
# loader = instagramDownloader(username)
# profile = loader.get_profile()
# print(profile)
# story = loader.get_story()
# print(story)

url = "http://www.instagram.com/p/CfM3SQDue6Y/?utm_source=ig_web_copy_link"
node = get_post(url)
print(node)

