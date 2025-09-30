# Use VSCode with specific extensions directory

## Command to use (replace <directory> by an actual project directory)

* code --extensions-dir="/pub/FISE_LAOA24/vscode" <directory>

## Extensions installed

* C/C++ de Microsoft
* C/C++ Extension Pack de Microsoft
* C/C++ Themes de Microsoft
* Make de twxs (juste pour le highlight du fichier CMakelist.txt)
* Make Tools de Microsoft
* Intellicode Completions de Microsoft
* TODO Highlight
* TODO Tree
	* change the Regex
		* File --> Preferences --> Settings --> Todo Tree --> Regex
		* Change the Todo-tree>Regex: Regex setting to (//|#|<!--|;|/\*|\*\s|^|^\s*(-|\d+.))\s*($TAGS)
