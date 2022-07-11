import os,sys,json

from downloader import InstagramExtractor, InstagramDownloader
from argparse import ArgumentParser

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
    optional
        -o       output_path
    """.format(argv0)

parser = ArgumentParser(description=__doc__, add_help=False, usage=usage_string(),
                            epilog="The complete documentation can be found at "
                                   "https://instaloader.github.io/.",
                            fromfile_prefix_chars='+')
parser.add_argument('--profile', nargs='*',
                           help="Download profile. If an already-downloaded profile has been renamed, Instaloader "
                                "automatically finds it by its unique ID and renames the folder likewise.")
parser.add_argument('--story', nargs='*',
                           help="Download profile. If an already-downloaded profile has been renamed, Instaloader "
                                "automatically finds it by its unique ID and renames the folder likewise.")
parser.add_argument('--post', nargs='*',
                           help="Download profile. If an already-downloaded profile has been renamed, Instaloader "
                                "automatically finds it by its unique ID and renames the folder likewise.")
parser.add_argument('-o', nargs='*',
                           help="Download profile. If an already-downloaded profile has been renamed, Instaloader "
                                "automatically finds it by its unique ID and renames the folder likewise.")
args = parser.parse_args()
if args.profile==None and args.story == None and args.post == None:
    print(usage_string())
    exit(0)
if args.profile != None and len(args.profile) != 1:
    print(usage_string())
    exit(0)
if args.story != None and len(args.story) != 1:
    print(usage_string())
    exit(0)
if args.post != None and len(args.post) != 1:
    print(usage_string())
    exit(0)

save_path = args.o[0] if args.o is not None else ""
loader = InstagramDownloader(save_path)

if args.profile != None:
    profile = loader.download_profile(args.profile[0])

    print(json.dumps(profile))
elif args.story != None:
    story = loader.download_story(args.story[0])
    print(json.dumps(story))
elif args.post != None:
    node = loader.download_post(args.post[0])
    print(json.dumps(node))

