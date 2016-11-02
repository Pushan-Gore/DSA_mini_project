#READ ME : Compression

Name: Pushan Gore  <br>
MIS:  111503023    <br>

	**Project : Compression** 
	
	The program uses the LZW compression algorithm (My modified version) to compress a file into a single file with
	".mtz" extension.
	The program can also compress a folder to a "single" folder with ".mtz" extension.
	".mtz" format stands for : My tar.gz
	I have used my modified version of LZW algorithm for this project.
	The algorithm reads characters from the file and writes them to the dictionary.
	The dictionary stores a character sequence which is not in the dictionary entry.

	Check "./project --help" for more information.
	The program takes a single file for compression and compresses it into a '.mtz' format file.(With same name)
	Example: 
	<addr> $ ./project -c example
	<addr> >> example.mtz will be created.
	The program also takes folders to compress as:
	Example:
		./project -dc <FOLDER>
		>> FOLDER.mtz will be created in the same directory as that of FOLDER.

	*Decompressing or extracting:

	
 
	Check "./project --help" for more information.
	The program takes a single file for decompressing or extracting. The file should have a '.mtz' extension.
	The outputed file will have the same name without the '.mtz' extension.
	Example:
		./project -x example.mtz
		>> example will be created.
	The program will also extract a ".mtz" extension folder.
	Example:
		./project -dx FOLDER.mtz
		>>FOLDER will be created in the same directory of FOLDER.mtz
	
	Limitations :
		The program takes some time to compress very large files.
		Although extracting takes less than a second.
		Also there is a slight problem with .pdf, .mp3  and some .jpg formats.
		All other formats work perfectly.
		An unknown character may occur while decompressing text file. ( Only a single character )
	

