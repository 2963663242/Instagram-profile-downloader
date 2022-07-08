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
if args.profile==None and args.stroy == None and args.stroy == None:
    print(usage_string())
    exit(0)
if args.profile != [] and len(args.profile) != 2:
    print(usage_string())
    exit(0)
if args.stroy != [] and len(args.stroy) != 2:
    print(usage_string())
    exit(0)
if args.posts != [] and len(args.posts) != 2:
    print(usage_string())
    exit(0)

if args.profile != None:
    loader = instagramDownloader(args.profile[1])
    profile = loader.get_profile()
    print(profile)
elif args.stroy != None:
    loader = instagramDownloader(args.stroy[1])
    story = loader.get_story()
    print(story)
elif args.posts != None:
    node = get_post(args.posts[1])
    print(node)

