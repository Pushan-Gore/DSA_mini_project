#READ ME : Compression

**** Pushan Gore  **** <br>
**** 111503023    **** <br>
**** Compression  **** <br>

	#Project : Compression (Modified Version of LZW)

	*Compressing:
	
	The program uses the LZW compression algorithm (My modified version) to compress a file into a single file with
	".mtz" extension.
	The program can also copress a folder in a "single" folder with ".mtz" extension.
	".mtz" format stands for : My tar.z
	I have used my modified version of LZW algorithm for this project.
	The algorithm reads characters from the file and writes them to the dictionary.
	
	Compression algorithm is:
   	BEGIN:
		s = next input character
		while not EOF
			{ c = next input character
			  if s + c exists in dictionary 
				s = s + c
			  else {
				output the code for s;
				add string s + c to the dictionary with a new code
				s = c
			  }
			}
		output the code for s
   	END

	Check "./project --help" for more information.
	The program takes a single file for compression and compresses it into a '.mtz' format file.(With same name)
	Example: 
		./project -c example
		>> example.mtz will be created.
	The program also takes folders as:
	Example:
		./project -dc <FOLDER>
		>> FOLDER.mtz will be created in the same directory as that of FOLDER.

	*Decompressing or extracting:

	Extraction algorithm is :
 	  BEGIN:
		s = NILL
   		while not EOF {
			k = next input code
			entry = dictionary entry for k
			//exception case
			if(entry == NULL)
				entry = s + s[0]
			output entry
			if(s != NILL) 
				add string s + entry[0] to dictionary with a new code
			s = entry
		}
	   END
 
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
	

