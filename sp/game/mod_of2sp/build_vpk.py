# User settings (don't use the \ character)

# Script begins
import os,subprocess,sys

file_types = [ "vmt", "vtf", "mdl", "phy", "vtx", "vvd", "vcd", "wav", "bsp", "pcf" ]

vpk_path = sys.argv[1]
vpkfile_name = sys.argv[2]
target_folders = sys.argv[3:]

from os.path import join
list_filename = vpkfile_name + "_" + "vpk_list.txt"
response_path = join(os.getcwd(),list_filename)
 
out = open(response_path,'w')
len_cd = len(os.getcwd()) + 1
 
for user_folder in target_folders:
	for root, dirs, files in os.walk(join(os.getcwd(),user_folder)):
		for file in files:
			if len(file_types) and file.rsplit(".")[-1] in file_types:
				out.write(os.path.join(root[len_cd:].replace("/","\\"),file) + "\n")
 
out.close()
 
subprocess.call([vpk_path, "-M", "a", vpkfile_name, "@" + response_path])
