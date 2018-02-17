#!/usr/bin/python
# Contributor : McKay Davis

import os
import re
import sys
import select 
import subprocess

def write_data_log(data_log):
	filename = "nm-otool-checker.txt";
	with open(filename, 'w') as f: 
		f.write(data_log);
	os.system("open " + filename);

def diff(first, second):
	# Consider both outputs with only one line as an error message
	if (len(first) == 1 and len(second) == 1):
		return ([]);
	second = set(second);
	return ([item for item in first if item not in second]);

def get_diff_to_string(diff_list, char):
	diff_str = str();
	for e in diff_list:
		diff_str += char + ' ' + e + '\n';
	if char == '<':
		diff_str += "--------" + '\n';
	return (diff_str);

def clean_data(stdout):
	lines = stdout.splitlines();
	char = ('' if ("--remove-space" in sys.argv) else ' ');
	for i, line in enumerate(lines):
		str_trimmed = re.sub("\s+", char, line).strip();
		if (str_trimmed == ""):
			del (lines[i]);
		else:
			lines[i] = str_trimmed;
	return (lines);

def async_exec(bin, args):
	# Create a buffer and a pipe to receive stdout and stderr from process
	(pipe_r, pipe_w) = os.pipe();
	buffer = str();
	# Call subprocess
	process = subprocess.Popen([bin, args],
                               shell  = False,
                               stdout = pipe_w,
                               stderr = pipe_w);
	# Loop while the process is executing
	while (process.poll() is None):
		while (len(select.select([pipe_r], [], [], 0)[0]) == 1):
			buffer += os.read(pipe_r, 4096);
	# Cleanup
	os.close(pipe_r)
	os.close(pipe_w)
	return (clean_data(buffer));


def exec_tests(data):
	index = 0;
	data_log = str();
	while (index < len(data)):
		for directory in data[index]:
			os.write(1, directory + ': ');
			correct = 0;
			for path in data[index][directory]:

				nm_sys_output = async_exec("nm", path);
				nm_usr_output = async_exec("./ft_nm", path);

				diff_sys = diff(nm_sys_output, nm_usr_output);
				diff_usr = diff(nm_usr_output, nm_sys_output);

				if (not diff_sys and not diff_usr):
					correct += 1;
					data_log += path + "    [Success]\n";	#\u2705
				else:
					data_log += path + "    [Error]\n";		#\u2b55

				data_log += get_diff_to_string(diff_sys, '<');
				data_log += get_diff_to_string(diff_usr, '>');

			print str(correct) + " / " + str(len(data[index][directory]));
		index += 1;
	write_data_log(data_log);

def	main():
	data = list();
	dirs =\
	[
		"/bin",
		"/usr/bin",
		"/usr/lib/",
		"./unittest/32",
		"./unittest/64",
		"./unittest/ar"
#	"correc"
	];
	for directory in dirs:
		files = list();
		for filename in os.listdir(directory):
			path = directory + '/' + filename;
			if os.path.isfile(path) and os.access(path, os.X_OK):
				files.append(path);
		data.append({directory : files});
	exec_tests(data);

if __name__ == '__main__':
	main();

# This tool compare the data difference's on two outputs.
# No order or repetition are make in consideration.
# Sometimes the buffer are cut with the whitespace padding and can generate a false positive.
# Please comment the padding in your source code or use the option --remove-space.
# ./nm-otool-checker/checker.py --remove-space (Note: With this option the checker log is less readable).

# Author: Ibouchla