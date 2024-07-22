#!/bin/bash

# Author           : Matsvei Kasparovich ( mkasporpol@gmail.com )
# Created On       : 18.05.24
# Last Modified By : Matsvei Kasparovich ( mkasporpol@gmail.com )
# Last Modified On : 20.05.24 
# Version          : 1.0
#
# Description      : A script to zip, crypt and share files and directories
# Opis            : Skrypt do zipowania, szyfrowania i udostępniania plików i katalogów
#
# Licensed under GPL (see /usr/share/common-licenses/GPL for more details
# or contact # the Free Software Foundation for a copy)

VERSION="1.0"

# error handling
handle_error() {
    local message=$1
    echo "Error: $message" >&2
    exit 1
}

# check if a command is installed
check_command() {
    if ! [ -x "$(command -v "$1")" ]; then
        handle_error "$1 is not installed"
    fi
}

# check if dependencies are installed
check_command dialog
check_command openssl
check_command jq
check_command curl
check_command zip


# configure function
configure() {
    cryptType=$(grep cryptType ~/.config/zipshare/config | cut -d'=' -f2)
    shareSystem=$(grep shareSystem ~/.config/zipshare/config | cut -d'=' -f2)
    dropBoxApiKey=$(grep dropBoxApiKey ~/.config/zipshare/config | cut -d'=' -f2)
    while true; do
        dialog --keep-tite --stdout --title "Configure" --clear --menu "Choose an option" 0 0 0 \
            1 "Crypt Type" \
            2 "Share System" \
            3 "DropBox API Key" \
            4 "Exit" > /tmp/zipshare_config
        choice=$(cat /tmp/zipshare_config)
        case $choice in
            1)
                cryptType=$(dialog --keep-tite --stdout --title "Crypt Type" --clear --radiolist "Choose default crypt type" 0 0 0 unencrypted "Unencrypted" on aes "AES" off des "DES" off blowfish "Blowfish" off) || handle_error "Error getting crypt type"
                ;;
            2)
                shareSystem=$(dialog --keep-tite --stdout --title "Share System" --clear --radiolist "Choose default share system" 0 0 0 file.io "File.io" on  dropbox "DropBox" off) || handle_error "Error getting share system"
                ;;
            3)
                dropBoxApiKey=$(dialog --keep-tite --stdout --title "DropBox API Key" --clear --inputbox "Enter DropBox API Key" 0 0) || handle_error "Error getting DropBox API Key"
                ;;
            4)
                echo "cryptType=$cryptType" > ~/.config/zipshare/config
                echo "shareSystem=$shareSystem" >> ~/.config/zipshare/config
                echo "dropBoxApiKey=$dropBoxApiKey" >> ~/.config/zipshare/config
                break
                ;;
        esac
    done
}



# check if config file exists
if [ ! -f ~/.config/zipshare/config ]; then
    mkdir -p ~/.config/zipshare
    touch ~/.config/zipshare/config
    configure
fi

cryptType=$(grep cryptType ~/.config/zipshare/config | cut -d'=' -f2)
shareSystem=$(grep shareSystem ~/.config/zipshare/config | cut -d'=' -f2)
dropBoxApiKey=$(grep dropBoxApiKey ~/.config/zipshare/config | cut -d'=' -f2)


# args
args=("$@")
filesNdirs=()
crypt=false
share=false
zipPassw=false
name=false
nameStr=""

for arg in "${args[@]}"; do
    case $arg in
        -h|--help)
            echo "Usage: $0 [options]"
            echo "Example: $0 -c aes -s file.io -p -n myzip file1 file2 dir1"
            echo "Options:"
            echo "  -h, --help    Show this help message and exit"
            echo "  -v, --version Show version and author"
            echo "  -c, --crypt   Crypt type, default unencrypted, options: unencrypted, aes, des, blowfish"
            echo "  -s, --share   Share system, default file.io, options: file.io, dropbox"
            echo "  -p, --password Password for crypting zip file"
            echo "  -n, --name    Name of the zip file"
            echo "  -configure    Configure system"
            exit 0;
            ;;
        -c|--crypt)
            # next arg is the crypt type
            crypt=true
            ;;
        -s|--share)
            # next arg is the share type
            share=true
            ;;
        -p|--password)
            # next arg is the password
            zipPassw=true
            ;;
        -configure)
            # configure system
            configure
            ;;
        -n|--name)
            # next arg is the name
            name=true
            ;;
        -v|--version)
            echo "Version: $VERSION"
            echo "Author: Matsvei Kasparovich"
            exit 0;
            ;;
        *)
            if [ "$crypt" = true ]; then
                cryptType="$arg"
                crypt=false
                continue
            fi
            if [ "$share" = true ]; then
                shareSystem="$arg"
                share=false
                continue
            fi
            if [ "$name" = true ]; then
                nameStr="$arg"
                name=false
                continue
            fi
            filesNdirs+=("$arg")
            ;;
    esac
done

# check if filesNdirs is empty
if [ ${#filesNdirs[@]} -eq 0 ]; then
    handle_error "No files or directories provided"
fi

# check if filesNdirs exist
for fileNdir in "${filesNdirs[@]}"; do
    if [ ! -e "$fileNdir" ]; then
        handle_error "$fileNdir does not exist"
    fi
done

# check if cryptType is valid
if [ "$cryptType" != "" ]; then
    case $cryptType in
        unencrypted|aes|des|blowfish)
            ;;
        *)
            handle_error "Invalid crypt type"
            ;;
    esac
fi

# check if shareSystem is valid
if [ "$shareSystem" != "" ]; then
    case $shareSystem in
        file.io|dropbox)
            ;;
        *)
            handle_error "Invalid share system"
            ;;
    esac
fi

# zip files and directories
zipFile="/tmp/$(date +%s).zip"
if [ "$zipPassw" = true ]; then
    password=$(dialog --keep-tite --stdout --title "Password" --clear --insecure --passwordbox "Enter zip password" 0 0) || handle_error "Error getting zip password"
    zip -P "$password" -r "$zipFile" "${filesNdirs[@]}" handle_error "Error zipping files and directories"
else
    zip -r "$zipFile" "${filesNdirs[@]}" || handle_error "Error zipping files and directories"
fi
echo "Files and directories zipped successfully under $zipFile"

# crypt zip file
if [ "$cryptType" != "unencrypted" ]; then
    # check name is set
    if [ "$nameStr" = "" ]; then
        nameStr=$(date +%s)
    fi
    cryptFile="/tmp/$nameStr.zip.$cryptType"
    password=$(dialog --keep-tite --stdout --title "Password" --clear --insecure --passwordbox "Enter crypt password" 0 0) || handle_error "Error getting crypt password"
    case $cryptType in
        aes)
            openssl enc -aes-256-cbc -salt -in "$zipFile" -out "$cryptFile" -k "$password" || handle_error "Error crypting zip file"
            ;;
        des)
            openssl enc -des-cbc -salt -in "$zipFile" -out "$cryptFile" -k "$password" || handle_error "Error crypting zip file"
            ;;
        blowfish)
            openssl enc -bf-cbc -salt -in "$zipFile" -out "$cryptFile" -k "$password" || handle_error "Error crypting zip file"
            ;;
    esac
    echo "Zip file crypted successfully under $cryptFile"
    zipFile=$cryptFile
else 
    if [ "$nameStr" != "" ]; then
        mv "$zipFile" "/tmp/$nameStr.zip"
        zipFile="/tmp/$nameStr.zip"
    fi
fi

# share zip file
case $shareSystem in
    wetransfer)

        ;;
    file.io)
        resp=$(curl -F "file=@$zipFile" https://file.io)
        # check if success
        if [ "$(echo "$resp" | jq -r '.success')" != "true" ]; then
            handle_error "Error sharing file"
        fi
        # get file link
        fileLink=$(echo "$resp" | jq -r '.link')
        echo "File shared successfully under $fileLink"
        ;;
    dropbox)
        # check if dropBoxApiKey is set
        if [ "$dropBoxApiKey" = "" ]; then
            handle_error "DropBox API Key is not set"
        fi
        # upload file to dropbox
        resp=$(curl -X POST https://content.dropboxapi.com/2/files/upload \
            --header "Authorization: Bearer $dropBoxApiKey" \
            --header "Dropbox-API-Arg: {\"path\": \"/$(basename "$zipFile")\",\"mode\": \"add\",\"autorename\": true,\"mute\": false}" \
            --header "Content-Type: application/octet-stream" \
            --data-binary @"$zipFile")
        # check if success
        if [ "$(echo "$resp" | jq -r '.path_lower')" = "" ]; then
            handle_error "Error sharing file"
        fi

        # get file link
        fileLink="https://www.dropbox.com/home/Apps/zipshare/$(basename "$zipFile")"
        echo "File shared successfully under $fileLink"

        #share 
        resp=$(curl -X POST https://api.dropboxapi.com/2/files/get_temporary_link \
            --header "Authorization: Bearer $dropBoxApiKey" \
            --header "Content-Type: application/json" \
            --data "{\"path\": \"/$(basename "$zipFile")\"}")
        # check if success
        if [ "$(echo "$resp" | jq -r '.link')" = "" ]; then
            handle_error "Error sharing file"
        fi
        # get file link
        fileLink=$(echo "$resp" | jq -r '.link')
        # fileLink="https://www.dropbox.com/home/Apps/zipshare/$(basename "$zipFile")"
        echo "File shared successfully under $fileLink"
        ;;
esac