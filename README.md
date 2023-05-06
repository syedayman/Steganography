# Steganography

Steganography is a technique for hiding text in images, by replacing successive random pixels in the image with letters from the text. The text is then retrieved by comparing the new image with the old image, and extracting letters from the new one where it differs from the old one.
This program works with PPM images with the P3 format only
The program is designed as follows; first the necessary structures needed to hold the ppm images have been created and they are :

struct Pixel : to hold rbg values of pixels of the PPM images

struct Comments : a linked list implemented to the comments in the PPM images

struct PPM : to hold the contents of a P3 format PPM file

copyLL : to deepcopy the linked list holding the comments

copyPPM : to deepcopy the PPM structure

loadPPMfromFile : to load a file and return it and show error if file cant be found or isnt of valid PPM format

getPPM : first creates a file pointer and allocates space to it based on the PPM format using the malloc keyword and PPM struct defined earlier, then one by one the contents of the ppm file are scanned and stored in a file first it checks if the PPM is of cirrect format then it checks if a line starts with '#' indicating that it is a comment if so it parses it into the linked list if not it proceeds to the scan the other contents of the PPM file including the pixel array holding every individiaul RGB value

showPPM : it prints a PPM struct in the same format as a PPM file simply by printing all the contents of the PPM file that it takes as arguments one by one
encode : it first initializes a pointer to point to the PPM struct created by the copyPPM function and then a random number generator has been defined in such a way that the value generated by it is niether too small (+100) nor too large (%100). Then it takes the encoded message which is a parameter and parses it into random pixel values successively so that the text is still in correct order when decoding

decode : the decode function takes two PPM structs as argument one is the encodedPPM and the other is the comparisionPPM which we will use to compare the red pixels to each other and if any difference is found the value of the changed red pixel is stored in the decoded messsage char pointer and since it has been encoded in the correct order it will be deocded in the correct order and then we append the end of string constant to it
and then we have the main function of the program which works by the following calls

For encoding
$ ./steg e input.ppm > output.ppm
where 'e' is to denote that we will be encoding a mesage into the input file input.ppm and the output is directed to output.ppm which is created if it doesnt exist

For decoding
$ ./steg d input.ppm output.ppm
where 'd' is to denote decoding. The message hidden in output.ppm after decoding by comparison with input.ppm is displayed
