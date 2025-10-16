// DELETE button handler for the Simple Test section
document.getElementById('deleteBtn').addEventListener('click', function() {
    var path = document.getElementById('pathInput').value.trim();
    var resultElement = document.getElementById('result');
  
    if (!path) {
      resultElement.textContent = 'Veuillez entrer un chemin.';
      return;
    }
    if (path[0] !== '/') path = '/' + path;
  
    var url = window.location.origin + path;
    resultElement.textContent = 'URL for DELETE : ' + url + '...';
  
    fetch(url, { method: 'DELETE' })
      .then(resp => resp.text().then(text => {
        resultElement.textContent = 'Response : ' + resp.status + ' ' + resp.statusText + '\n' + text;
      }))
      .catch(err => {
        resultElement.textContent = 'Error !!!';
      });
  });
  
  // File Upload Handler (Stays here for form submission control)
  async function handleFileUpload(event) {
      event.preventDefault(); // Stop the default form submission
  
      const form = event.target;
      // NOTE: form.action is currently empty in the HTML, so this is likely a placeholder.
      // Assuming the user will modify the HTML or that the endpoint is derived elsewhere.
      const endpoint = form.action || window.location.origin + '/upload'; // Placeholder endpoint
      // NOTE: A result box with id 'uploadResult' was not in the original HTML, 
      // so this part might require HTML modification for proper display. Using a placeholder for now.
      const resultBox = document.getElementById('uploadResult'); 
      
      if (resultBox) {
          resultBox.textContent = 'Uploading file...';
      } else {
          console.warn("Result box with ID 'uploadResult' not found.");
      }
  
      try {
          // Use the FormData object to correctly handle the file input
          const formData = new FormData(form);
  
          const response = await fetch(endpoint, {
              method: 'POST',
              body: formData // Fetch sends multipart/form-data correctly
          });
  
          const contentType = response.headers.get("content-type");
          
          if (resultBox) {
              if (contentType && contentType.includes("application/json")) {
                  const data = await response.json();
                  resultBox.textContent = JSON.stringify(data, null, 2);
              } else {
                  const text = await response.text();
                  resultBox.textContent = `Non-JSON Response (${response.status}):\n${text}`;
              }
          } else {
               // Log to console if no result box is available
               const text = await response.text();
               console.log(`Upload Response (${response.status}): ${text}`);
          }
  
  
      } catch (error) {
          if (resultBox) {
              resultBox.textContent = 'Request failed: ' + error.message;
          } else {
              console.error('Request failed:', error.message);
          }
      }
  }
  
  // Attach the file upload handler to the form
  // The form in the HTML has id="form"
  document.getElementById('form').addEventListener('submit', handleFileUpload);
  
  
  // Original template script functionality (menu toggle, etc.)
  const menuToggle = document.getElementById('menuToggle');
  const navLinks = document.getElementById('navLinks');
  
  menuToggle.addEventListener('click', () => {
      navLinks.classList.toggle('active');
      menuToggle.classList.toggle('active');
  });
  
  // Close menu when a link is clicked
  document.querySelectorAll('.nav-link').forEach(link => {
      link.addEventListener('click', () => {
          navLinks.classList.remove('active');
          menuToggle.classList.remove('active');
      });
  });