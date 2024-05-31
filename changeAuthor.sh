#!/bin/sh

OLD_NAME="Goran Fiinić"
OLD_EMAIL="goranfilinic@gmail.com"
CORRECT_NAME="Goran Filinić"
CORRECT_EMAIL="goranfilinic@gmail.com"

git filter-repo --force --commit-callback '
import codecs
OLD_NAME = codecs.decode(b"Goran Fiini\xc4\x87", "utf-8")
OLD_EMAIL = "goranfilinic@gmail.com"
CORRECT_NAME = codecs.decode(b"Goran Filini\xc4\x87", "utf-8")
CORRECT_EMAIL = "goranfilinic@gmail.com"

if commit.author_name == OLD_NAME:
    commit.author_name = CORRECT_NAME

if commit.committer_name == OLD_NAME:
    commit.committer_name = CORRECT_NAME

'

