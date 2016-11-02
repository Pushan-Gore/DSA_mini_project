#READ ME : Compression

Name: Pushan Gore  <br>
MIS:  111503023    <br>

**Project : Compression** 

The program uses the LZW compression algorithm (My modified version) to compress a file into a single file with
".mtz" extension. <br>
The program can also compress a folder to a "single" folder with ".mtz" extension. <br>
".mtz" format stands for : My tar.gz <br>
I have used my modified version of LZW algorithm for this project. <br>
The algorithm reads characters from the file and writes them to the dictionary. <br>
The dictionary stores a character sequence which is not in the dictionary entry. <br>
<br>
The program takes a single file for compression and compresses it into a '.mtz' format file.(With same name) <br>
Example: <br>
      
    $ ./project -c example 
    >> example.mtz will be created. 
The program also takes folders to compress as: <br>
Example: <br>
  <addr> $ ./project -dc FOLDER <br>
  <addr> >> FOLDER.mtz will be created in the same directory as that of FOLDER. <br>
<br>
Check "./project --help" for more information. <br>
<br>
For decompression or extraction, the program takes a single file or a single folder. The file or folder should have a '.mtz' extension. <br>
The outputed file will have the same name without the '.mtz' extension. <br>
Example: <br>
<addr> $ ./project -x example.mtz <br>
<addr> >> example will be created. <br>
<br>
The program will also extract a ".mtz" extension folder. <br>
Example: <br>
<addr> $ ./project -dx FOLDER.mtz <br>
<addr> >>FOLDER will be created in the same directory of FOLDER.mtz <br>
<br>
Limitations : <br>
The program takes some time to compress very large files. <br>
Although extracting takes less than a second. <br>
Also there is a slight problem with .pdf, .mp3  and some .jpg formats.	A "single" unknown character may occur while decompressing text file. <br>
All other formats work perfectly. <br>


