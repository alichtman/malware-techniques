#!/usr/bin/env bash

# This script gives you the option to take a screenshot
# or a video of the desktop, for a specified duration.
#
# Tested on macOS 10.14.4

screenshot_path="$HOME/Desktop/malware_screenshot.png"
video_path="$HOME/Desktop/malware_video.mp4"

# This script will only work on macOS
if [ "$(uname)" == "Darwin" ]; then
	# Prompt for video or screenshot.
	printf "macOS Screen-capture Demo\n\n"

	options=("Screenshot" "Video" "Clean up")
	PS3="Pick an option: "
	select opt in "${options[@]}" "Quit"; do

		case "$REPLY" in
			# Screenshots are easy. Take it and dump it on the desktop.
		1 ) echo "Screenshot can be found at: $screenshot_path"; screencapture -C -x "$screenshot_path";;
			# Prompt for video duration, then take the video and dump it on the desktop after silencing the output.
		2 ) echo -n "Enter video duration (seconds): ";
			read video_seconds
			# Check that video length is reasonable.
			if [[ $video_seconds -lt 0 || $video_seconds -gt 200 ]]
				then
					echo "Input outside acceptable range."
			else
				video_cmd="screencapture -C -V $video_seconds -x $video_path"
				$video_cmd &>/dev/null; echo "Done! Video can be found at: $video_path"
			fi;;
		3 ) echo "Cleaning up created files...";
			# If no files have been removed, display a notification.
			files_removed=0
			if [ -f "$screenshot_path" ] ; then
				rm -v "$screenshot_path"
				# Does anyone else really miss Python right now? files_removed += 1? Anyone?
				files_removed=$((files_removed + 1))
			fi

			if [ -f "$video_path" ] ; then
				rm -v "$video_path"
				files_removed=$((files_removed + 1))
			fi
			
			if [ $files_removed == 0 ] ; then
				echo "No files to clean up."
			fi;;

			# Exit.
		$(( ${#options[@]}+1 )) ) echo "Exiting."; break;;
		*) echo "Invalid option. Try another one: ";continue;;

		esac
		break
	done

		
else
	echo "ERROR: This script will only work on macOS."
fi
