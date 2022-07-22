import os,sys,json

from downloader import InstagramExtractor, InstagramDownloader
from argparse import ArgumentParser
from inslog import logger, init
from utils import flush_print


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
        -l       log_path
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
parser.add_argument('-l', nargs='*',
                           help="Download profile. If an already-downloaded profile has been renamed, Instaloader "
                                "automatically finds it by its unique ID and renames the folder likewise.")
args = parser.parse_args()

log_path = args.l[0] if args.l is not None else ""
save_path = args.o[0] if args.o is not None else ""

init(log_path)

logger.info("start accepting parameters--------------------------")

logger.info("save_path == %s" % (os.path.abspath(save_path)))
logger.info("log_path == %s" % (os.path.abspath(log_path)))

if args.profile==None and args.story == None and args.post == None:
    logger.error("no --profile --story --post")
    print(usage_string())
    exit(0)
if args.profile != None and len(args.profile) != 1:
    logger.error("have --profile but no username")
    print(usage_string())
    exit(0)
if args.story != None and len(args.story) != 1:
    logger.error("have --story but no username")
    print(usage_string())
    exit(0)
if args.post != None and len(args.post) != 1:
    logger.error("have --post but no url")
    print(usage_string())
    exit(0)

loader = InstagramDownloader(save_path)


if args.profile != None:
    logger.info("Your selected function is profile, The username for the query is %s" % args.profile[0])
    ret = loader.download_profile(args.profile[0])
    flush_print(json.dumps(ret))

elif args.story != None:
    logger.info("Your selected function is story, The username for the query is %s" % args.story[0])
    ret = loader.download_story(args.story[0])
    flush_print(json.dumps(ret))

elif args.post != None:
    logger.info("Your selected function is post, The url for the query is %s" % args.post[0])
    ret = loader.download_post(args.post[0])
    flush_print(json.dumps(ret))

if(ret['msg']['ret_code'] == '0'):
    logger.info(" Parsing succeeded")
else:
    logger.error(" download failed, error code == %s , exception == %s" % (ret['msg']['ret_code'],ret['msg']['exception']))
logger.info("--------------------------inslog execution end")