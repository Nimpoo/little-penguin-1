#! /bin/bash

# ? Don't forget to launch this command for granting access to users for `/sys/kernel/debug` directory:
### ?
#   ? sudo chmod o+rx /sys/kernel/debug
### ?

prompt() {
	tput bold
	tput setaf "$1"
	echo "$2"
	tput sgr0
	read -n 1 -s -r -p ""
	echo ""
}

show_command() {
	tput setaf 5
	printf '  $ %s\n' "$1"
	tput sgr0
}

prompt 6 "Before launching the tests, type this command: \"sudo chmod o+rx /sys/kernel/debug\". (Press any key to continue…)"

prompt 6 "These tests should normally be launched as a regular user. The \`id\` and \`jiffies\` permissions are checked this way, while the root-only \`foo\` write should return \"Permission denied\". Press any key to continue…"

prompt 6 "If the script itself is not executable, run: \`chmod +x assignment07/test.sh\`. To launch the complete script as root, run \`sudo ./assignment07/test.sh\`; this grants access to the root-only write but no longer checks its user restriction. Press any key to continue…"

prompt 6 "Between each command, press any key to continue and inspect the result."

prompt 4 "Check if the directory exists. Press any key to continue…"

# Check if the directory exists
show_command "ls /sys/kernel/debug"
ls /sys/kernel/debug
echo ""

prompt 4 "Check if the files \`id\`, \`jiffies\` and \`foo\` exist. Press any key to continue…"

# Check if the files `id`, `jiffies` and `foo` exists
show_command "ls /sys/kernel/debug/fortytwo"
ls /sys/kernel/debug/fortytwo
echo ""

# * `id` file
prompt 2 "Check if the \`id\` file is readable by everyone. Press any key to continue…"

# Check if the `id` file is readable by everyone
show_command "cat /sys/kernel/debug/fortytwo/id"
cat /sys/kernel/debug/fortytwo/id
echo ""

prompt 3 "Check if the \`id\` file is writable by everyone and returns no errors. Press any key to continue…"

# Check if the `id` file is writable by everyone + if it return with no errors
show_command "echo -n \"mayoub\" > /sys/kernel/debug/fortytwo/id"
echo -n "mayoub" > /sys/kernel/debug/fortytwo/id
echo ""

prompt 1 "Check the same write with an invalid value. An error is expected. Press any key to continue…"

# Same but if it returns an error
show_command "echo \"Garp\" > /sys/kernel/debug/fortytwo/id"
echo "Garp" > /sys/kernel/debug/fortytwo/id
echo ""

# * `jiffies` file
prompt 2 "Check if the \`jiffies\` file is readable by everyone. Press any key to continue…"

# Check if the `jiffies` file is readable by everyone
show_command "cat /sys/kernel/debug/fortytwo/jiffies"
cat /sys/kernel/debug/fortytwo/jiffies
echo ""

prompt 3 "Check if the \`jiffies\` file is writable by no one. Press any key to continue…"

# Check if the `jiffies` file is writable by no one
show_command "echo \"TEKKEN 8\" > /sys/kernel/debug/fortytwo/jiffies"
echo "TEKKEN 8" > /sys/kernel/debug/fortytwo/jiffies
echo ""

# * `foo` file
prompt 3 "Check if the \`foo\` file is writable root-only. Press any key to continue…"

# Check if the `foo` file is writable root-only
show_command "echo \"Marvel Tōkon\" > /sys/kernel/debug/fortytwo/foo"
echo "Marvel Tōkon" > /sys/kernel/debug/fortytwo/foo
echo ""

prompt 2 "Check if the \`foo\` file is readable by everyone. Press any key to continue…"

# Check if the `foo` file is readable by everyone
show_command "cat /sys/kernel/debug/fortytwo/foo"
cat /sys/kernel/debug/fortytwo/foo
echo ""

prompt 4 "Check if the \`foo\` file has a limit of 4096 bytes (PAGE_SIZE) per buffer. Press any key to continue…"

# Check if the `foo` file has a limit of 4096 bytes (PAGE_SIZE) per buffer
show_command "echo \$(python3 -c \"print('a' * 4096)\") > /sys/kernel/debug/fortytwo/foo"
echo $(python3 -c "print('a' * 4096)") > /sys/kernel/debug/fortytwo/foo
echo ""

prompt 4 "Check if the \`foo\` file can be written with fewer than 4096 bytes (PAGE_SIZE). Press any key to continue…"

# Check if the `foo` file can be written with lower than 4096 bytes (PAGE_SIZE)
show_command "echo \$(python3 -c \"print('a' * 4095)\") > /sys/kernel/debug/fortytwo/foo"
echo $(python3 -c "print('a' * 4095)") > /sys/kernel/debug/fortytwo/foo
echo ""
