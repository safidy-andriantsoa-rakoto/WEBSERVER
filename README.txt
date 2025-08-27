Redirection :
- Add "redirect" + statusCode + newURI inside the location bloc
- [Optionnal] Add new location bloc to map the new root to get the files that needed the redirection

Custom Error page:
- Add "error_page" + list of all statusCode + exact error file path + add proper location for the custom page
NB : You need to edit manually all the links inside the error page if you want to use or import exterior files

use : location /custom | root ./errorPage/custom | method GET