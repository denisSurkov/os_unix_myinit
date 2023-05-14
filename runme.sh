#!/bin/bash

CONFIG_FILENAME="/tmp/myinit-config"

function debugLog() {
    echo -e "\e[93m[~]\e[0m\t$1\n"
}

function successLog() {
  echo -e "\e[32m[+]\e[0m\t$1\n"

}

function failedLog() {
    echo -e "\e[31m[!]\e[0m\t$1\n"
}


function check3Processes() {
  debugLog "Checking started 3 processes"
  count=$(ps aux | grep -c -E "/tmp/custom-(a|b|c)")
  if [[ "$count" -eq 3 ]]; then
    successLog "Checked 3 processes, all working!"
  else
    failedLog "Failed to check 3 processes, $count"
  fi
}

function check1Process() {
  debugLog "Checking started 1"
  count=$(ps aux | grep -c -e "/tmp/custom-(a|b|c)")
  if [ "$count" -eq 1 ]; then
    successLog "Checked 1 processes, all working!"
  else
    failedLog "Failed to check 1 processes"
  fi
}

debugLog "Building app"
make
echo ""


debugLog "Deleting /tmp/myinit.log if exists"
rm /tmp/myinit.log 2> /dev/null


sleepPath=$(which sleep)
debugLog "Creating aliases for ${sleepPath}"
ln -s "$sleepPath" /tmp/custom-a
ln -s "$sleepPath" /tmp/custom-b
ln -s "$sleepPath" /tmp/custom-c

debugLog "Creating in & out files"
touch /tmp/custom-in
touch /tmp/custom-out

debugLog "All tmp files created"

debugLog "Creating config $CONFIG_FILENAME"
echo -e "/tmp/custom-a 1000 /tmp/custom-in /tmp/custom-out" > $CONFIG_FILENAME
echo -e "/tmp/custom-b 1500 /tmp/custom-in /tmp/custom-out" >> $CONFIG_FILENAME
echo -e "/tmp/custom-c 1000 /tmp/custom-in /tmp/custom-out" >> $CONFIG_FILENAME

debugLog "Contents of $CONFIG_FILENAME"
cat $CONFIG_FILENAME
echo ""

debugLog "Stating daemon"
./os_unix_myinit $CONFIG_FILENAME

debugLog "Sleeping 1 sec to allow daemon to run processes"
sleep 1

check3Processes

debugLog "Going to kill process 2"
pkill /tmp/custom-b

debugLog "Killed process 2, going to sleep 1 sec"
sleep 1

debugLog "Wake up, checking 3 processes"
check3Processes

debugLog "Changing config file"
echo -e "/tmp/custom-a 1000 /tmp/custom-in /tmp/custom-out" > $CONFIG_FILENAME

debugLog "Contents of $CONFIG_FILENAME"
cat $CONFIG_FILENAME
echo ""

debugLog "Sending SIGHUP signal"
pkill --signal SIGHUP os_unix_myinit

debugLog "Checking started /tmp/custom-a"
count=$(ps aux | grep -c "/tmp/custom-a")
if [ "$count" -eq 1 ]; then
  successLog "Checked /tmp/custom-a processes, it's working!"
else
  failedLog "Failed to check /tmp/custom-a processes"
fi

successLog "Logs content"
cat /tmp/myinit.log
echo ""

debugLog "Clean up"

pkill --signal SIGTERM myinit

unlink /tmp/custom-a
unlink /tmp/custom-b
unlink /tmp/custom-c

rm /tmp/custom-in
rm /tmp/custom-out
