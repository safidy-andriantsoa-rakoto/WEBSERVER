Redirection :
    - Add in the location bloc : [redirect] [statusCode] [newURI]
    - Optionnal :
        * newURI can contain "/$uri" at the end (ex: http://localhost:2003/tmp/$uri) to maintain the original URI research.
            ex : https://localhost:2003/images/file.jpg => https://localhost:2003/tmp/file.jpg ($uri == "file.jpg")
            Otherwise a listing directory if possible can be done
        * Add new location bloc to map the new root to get the files that needed by the redirection if placed locally

Custom Error page:
    - Add in the server bloc :  [error_page] [list of all statusCode] [page.html]

Upload (POST and DELETE) : 
    - default directory + default location bloc : use this if no custom directory is specified in the file configuration
    - custom directory must be named  : Add new location (must be upload/) bloc with specified root
