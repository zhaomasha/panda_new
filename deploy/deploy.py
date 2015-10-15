#########################################################################
# File Name: deploy.py
# Author: wangxin
# mail: wangxin4@iie.ac.cn
# Created Time: Tue 13 Oct 2015 04:45:44 PM CST
#########################################################################
#!/usr/local/bin/python
'''
Description:
'''
import os
import sys
import ConfigParser
base_dir = os.path.abspath(os.path.dirname(__file__))
sys.path.append(base_dir)

import config_proc

class Deployor(object):
    def __init__(self, sys_config_path, deploy_config_path):
        #init configer for deploy
        self.deploy_configer = ConfigParser.ConfigParser()
        self.deploy_configer.read(deploy_config_path)

        #init configer for system
        self.sys_configer = config_proc.DGSSConfiger(sys_config_path)

        self.slave_hosts = self.sys_configer.get("HOST_INFO","slave_ip").split()
        self.master_host = self.sys_configer.get("HOST_INFO","master_ip")

    def __install(self):
        os.system(os.path.join(base_dir,"base_install.sh"))
        username = self.deploy_configer.get("HOST_INFO","rootname")
        passwd = self.deploy_configer.get("HOST_INFO","rootpasswd")
        install_dir = self.deploy_configer.get("FILES","dst_home_dir")
        install_dir = os.path.join(install_dir,"install")
        for host in self.slave_hosts + [self.master_host]:
            cmd = os.path.join(base_dir,"install.sh")
            cmd = cmd + " %s %s %s %s"%(username, passwd, host, install_dir)
            os.system(cmd)

    def __copyfiles(self,dst_host,user_name, passwd, dst_dirs,dst_files ):
        '''
        dst_host: specify the destinate host for receiving files
        user_name: specify the dst_host's user name
        dst_dirs: directories that will be create on dst_host, in list type
        dst_files: files need to be copy to dst_host, in dictionary type { srcfile:dstfile,..   }
        '''
        mkdir_cmd = os.path.join(base_dir, "automkdir.exp")
        for dst_dir in dst_dirs:
            os.system("%s %s %s %s %s"%(mkdir_cmd, user_name, passwd, dst_host, dst_dir))
            
        scp_cmd = os.path.join(base_dir, "autoscp.exp")
        for dst_file in dst_files:
            src_file = dst_files[dst_file]
            os.system("%s %s %s %s %s %s"%(scp_cmd, user_name, passwd, dst_host, src_file, dst_file))

    def __file_deploy(self):

        envfile_dir = self.deploy_configer.get("FILES","envfile_dir_src")
        self.sys_configer.gen_env_files(envfile_dir)
        
        username = self.deploy_configer.get("HOST_INFO","rootname")
        passwd = self.deploy_configer.get("HOST_INFO","rootpasswd")

        #copy file for master
        dst_dirs = self.deploy_configer.get("FILES", "dst_dirs_master").split()
        dst_files_terms = self.deploy_configer.get("FILES","dst_files_master").split()
        dst_files = {}
        for term in dst_files_terms:
            dst,src = term.split('@')
            dst_files[dst] = src
        envfile_dst = self.deploy_configer.get("FILES","envfile_dst")
        envfile_dir_src = self.deploy_configer.get("FILES","envfile_dir_src")
        envfile_src = os.path.join(envfile_dir_src,"%s.envfile"%self.master_host)
        dst_files[envfile_dst] = envfile_src
        self.__copyfiles(self.master_host,username,passwd,dst_dirs,dst_files)

        #copy file for slaves
        for slave_host in self.slave_hosts:
            dst_dirs = self.deploy_configer.get("FILES", "dst_dirs_slave").split()
            dst_files_terms = self.deploy_configer.get("FILES","dst_files_slave").split()
            dst_files = {}
            for term in dst_files_terms:
                dst,src = term.split('@')
                dst_files[dst] = src
            envfile_dst = self.deploy_configer.get("FILES","envfile_dst")
            envfile_dir_src = self.deploy_configer.get("FILES","envfile_dir_src")
            envfile_src = os.path.join(envfile_dir_src,"%s.envfile"%slave_host)
            dst_files[envfile_dst] = envfile_src
            self.__copyfiles(slave_host,username,passwd,dst_dirs,dst_files)

    def deploy(self):

        self.__install()
        self.__file_deploy()

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print "invalid arg number ",len(sys.argv)
        print "usage: deploy.py system_config_path  deploy_config_path"
        sys.exit(1)
    sys_config_path = sys.argv[1]
    deploy_config_path = sys.argv[2]

    dp = Deployor(sys_config_path, deploy_config_path)
    dp.deploy()

