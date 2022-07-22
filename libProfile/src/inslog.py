import os,logging
from logging import handlers

logger = logging.getLogger('inslog')

def init(log_path ="",log_name = "inslog.txt", level = logging.DEBUG, when='D', backCount=1,fmt='%(asctime)s - %(pathname)s[line:%(lineno)d] - %(levelname)s: %(message)s'):
    log_path = os.path.abspath(log_path)
    filename =os.path.join(log_path,log_name)
    format_str = logging.Formatter(fmt)  # 设置日志格式
    logger.setLevel(level)  # 设置日志级别
    sh = logging.StreamHandler()  # 往屏幕上输出
    sh.setFormatter(format_str)  # 设置屏幕上显示的格式
    th = handlers.TimedRotatingFileHandler(filename=filename, when=when, backupCount=backCount,
                                           encoding='utf-8')  # 往文件里写入#指定间隔时间自动生成文件的处理
    th.setFormatter(format_str)  # 设置文件里写入的格式
    logger.addHandler(th)

