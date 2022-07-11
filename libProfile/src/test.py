import requests
from func_timeout import func_set_timeout
@func_set_timeout(10)
def downpic(url,filename):
    ret= requests.get(url)
    if ret.status_code == 200:
        with open(filename, 'wb') as fp:
            fp.write(ret.content)