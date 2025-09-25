Redirection :
    - Add in the location bloc : [redirect] [statusCode] [newURI]
    - Optionnal :
        * newURI can contain "/$uri" at the end (ex: http://localhost:2003/tmp/$uri) to maintain the original URI research.
            ex : https://localhost:2003/images/file.jpg => https://localhost:2003/tmp/file.jpg ($uri == "file.jpg")
            Otherwise a listing directory if possible can be done
        * Add new location bloc to map the new root to get the files that needed by the redirection if placed locally

Custom Error page:
    - Add in the server bloc :  [error_page] [list of all statusCode] [index.html file path]
    - Add new location bloc :
        location /custom 
        {
            root ./errorPage/$dir
            method GET
        }

        Where $dir = your file directory for custom page

    NB : You need to edit manually all the links inside the error page index.html if you want to use imported files using /custom/...

