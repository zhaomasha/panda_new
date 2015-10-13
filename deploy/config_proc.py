#########################################################################
# File Name: config_proc.py
# Author: wangxin
# mail: wangxin4@iie.ac.cn
# Created Time: Mon 12 Oct 2015 04:28:20 PM CST
#########################################################################
#!/usr/local/bin/python
'''
Description:
    Accorrding configuration file to generate system's env_file, and config
    the system on deploying.
    DGSS(Distributed Graph Storage System)
'''

import ConfigParser
import os

class DGSSConfiger(object):
    def __init__(self,config_path=""):
        '''
        initialize configer and read the configuration file if config_path not null
        '''
        self.configer = ConfigParser.ConfigParser()
        src_path = os.path.abspath(__file__)
        base_dir = os.path.dirname(src_path)
        if config_path!="":
            self.config_path=config_path
            self.read(config_path)
    
    def read(self, config_path):
        '''
        reload configuration according to file of config_path
        '''
        self.configer.read(config_path)

    def __gen_env_file(self, dst_host, dst_dir):
        '''
        description: Generate env file which will be applied on dst_host
        dst_host   : str that designate the destination host name 
        '''
        #open gen file
        envfile = open(os.path.join(dst_dir,"%s.envfile"%dst_host),'w')

        #write content of configuration to envfile
        envfile.write("#!/bin/bash\n")
        sections = self.configer.sections()
        for sec in sections:
            options = self.configer.options(sec)
            for option in options:
                value = self.get(sec,option)
                if option == "slave_ip":
                    slave_ips = value.split()
                    value = ':'.join(slave_ips)
                envfile.write("export %s='%s'\n"%(option.upper(),value))
        envfile.write("export LOCAL_IP=%s"%dst_host)
        envfile.close()

    def gen_env_files(self, dst_dir):
        '''
        Generate all env files that needed
        '''
        if not os.path.exists(dst_dir):
            os.system("mkdir -p %s"%dst_dir)
        slave_ips = self.get("HOST_INFO","slave_ip").split()
        master_ip = self.get("HOST_INFO","master_ip")
        for slave_ip in slave_ips:
            self.__gen_env_file(slave_ip, dst_dir)
        self.__gen_env_file(master_ip, dst_dir)

    def get(self, section, option):
        return self.configer.get(section, option)

    def prety_print(self):

        for sec in self.configer.sections():
            print sec
            for option in self.configer.options(sec):
                print "    %s=%s"%(option,self.configer.get(sec,option))
                



if __name__ == "__main__":

    config = DGSSConfiger('config/sample.config')
    config.prety_print()
    config.gen_env_files()






