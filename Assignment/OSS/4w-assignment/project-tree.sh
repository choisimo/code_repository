#!/bin/bash 

echo "Project tree:"

# Exclude common directories and files
tree -I 'docs|build|*.gz|*.txt|*.sh|.idea|node_modules|dist|public|backend-src|frontend-src' .