<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Test CGI/PHP - GET et POST</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
        }
        
        body {
            background-color: #f5f7fa;
            color: #333;
            line-height: 1.6;
            padding: 20px;
        }
        
        .container {
            max-width: 1200px;
            margin: 0 auto;
            background-color: white;
            border-radius: 10px;
            box-shadow: 0 0 20px rgba(0, 0, 0, 0.1);
            overflow: hidden;
        }
        
        header {
            background: linear-gradient(135deg, #6a11cb 0%, #2575fc 100%);
            color: white;
            padding: 30px;
            text-align: center;
        }
        
        h1 {
            font-size: 2.5rem;
            margin-bottom: 10px;
        }
        
        .subtitle {
            font-size: 1.2rem;
            opacity: 0.9;
        }
        
        .content {
            display: flex;
            flex-wrap: wrap;
            padding: 20px;
        }
        
        .section {
            flex: 1;
            min-width: 300px;
            padding: 20px;
            margin: 10px;
            background-color: #f9f9f9;
            border-radius: 8px;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.05);
        }
        
        h2 {
            color: #2575fc;
            margin-bottom: 15px;
            padding-bottom: 10px;
            border-bottom: 2px solid #eaeaea;
        }
        
        .method-badge {
            display: inline-block;
            padding: 4px 8px;
            border-radius: 4px;
            font-size: 0.8rem;
            font-weight: bold;
            margin-left: 10px;
        }
        
        .get-badge {
            background-color: #28a745;
            color: white;
        }
        
        .post-badge {
            background-color: #007bff;
            color: white;
        }
        
        .form-group {
            margin-bottom: 15px;
        }
        
        label {
            display: block;
            margin-bottom: 5px;
            font-weight: 600;
        }
        
        input, textarea, select {
            width: 100%;
            padding: 10px;
            border: 1px solid #ddd;
            border-radius: 4px;
            font-size: 1rem;
        }
        
        button {
            background: linear-gradient(135deg, #6a11cb 0%, #2575fc 100%);
            color: white;
            border: none;
            padding: 12px 20px;
            border-radius: 4px;
            cursor: pointer;
            font-size: 1rem;
            font-weight: 600;
            transition: all 0.3s ease;
            margin-right: 10px;
            margin-bottom: 10px;
        }
        
        button.get-btn {
            background: linear-gradient(135deg, #28a745 0%, #20c997 100%);
        }
        
        button.post-btn {
            background: linear-gradient(135deg, #007bff 0%, #0056b3 100%);
        }
        
        button:hover {
            transform: translateY(-2px);
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
        }
        
        .result {
            margin-top: 20px;
            padding: 15px;
            background-color: #e8f4fd;
            border-radius: 4px;
            border-left: 4px solid #2575fc;
        }
        
        .get-result {
            border-left-color: #28a745;
            background-color: #e8f5e8;
        }
        
        .post-result {
            border-left-color: #007bff;
            background-color: #e8f4fd;
        }
        
        .info-item {
            margin-bottom: 10px;
            padding: 8px;
            background-color: white;
            border-radius: 4px;
        }
        
        .data-display {
            background-color: #f8f9fa;
            padding: 15px;
            border-radius: 5px;
            margin-top: 15px;
            border-left: 4px solid #6c757d;
        }
        
        footer {
            text-align: center;
            padding: 20px;
            background-color: #f1f1f1;
            color: #666;
            font-size: 0.9rem;
        }
        
        #back {
            color: white;
            text-decoration: none;
            position: absolute;
            top: 20px;
            left: 20px;
            font-size: 1rem;
            background-color: rgba(0, 0, 0, 0.2);
            padding: 8px 12px;
            border-radius: 4px;
            transition: background-color 0.3s ease;

        }

        @media (max-width: 768px) {
            .content {
                flex-direction: column;
            }
            
            .section {
                min-width: 100%;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <header>
            <a href="/" id="back" >Home</a>
            <h1>Test Méthodes GET et POST</h1>
            <p class="subtitle">Comparaison des méthodes HTTP GET et POST en PHP</p>
        </header>
        
        <div class="content">
            <!-- Section Méthode GET -->
            <div class="section">
                <h2>Méthode GET <span class="method-badge get-badge">GET</span></h2>
                <p><strong>Caractéristiques :</strong></p>
                <ul style="margin-left: 20px; margin-bottom: 15px;">
                    <li>Données visibles dans l'URL</li>
                    <li>Limite de taille (environ 2048 caractères)</li>
                    <li>Peut être mis en cache</li>
                    <li>Idéal pour les recherches et filtres</li>
                </ul>
                
                <form method="get" action="">
                    <div class="form-group">
                        <label for="get_name">Nom:</label>
                        <input type="text" id="get_name" name="get_name" value="<?php echo isset($_GET['get_name']) ? htmlspecialchars($_GET['get_name']) : ''; ?>" placeholder="Votre nom">
                    </div>
                    
                    <div class="form-group">
                        <label for="get_city">Ville:</label>
                        <input type="text" id="get_city" name="get_city" value="<?php echo isset($_GET['get_city']) ? htmlspecialchars($_GET['get_city']) : ''; ?>" placeholder="Votre ville">
                    </div>
                    
                    <div class="form-group">
                        <label for="get_age">Âge:</label>
                        <input type="number" id="get_age" name="get_age" value="<?php echo isset($_GET['get_age']) ? htmlspecialchars($_GET['get_age']) : ''; ?>" placeholder="Votre âge">
                    </div>
                    
                    <button type="submit" class="get-btn" name="submit_get">Envoyer avec GET</button>
                    <button type="button" onclick="window.location.href = '<?php echo $_SERVER['PHP_SELF']; ?>'">Effacer GET</button>
                </form>
                
                <?php
                if (isset($_GET['submit_get'])) {
                    echo '<div class="result get-result">';
                    echo '<h3>Données reçues via GET:</h3>';
                    
                    if (!empty($_GET)) {
                        foreach ($_GET as $key => $value) {
                            if ($key !== 'submit_get' && !empty($value)) {
                                echo '<p><strong>' . htmlspecialchars($key) . ':</strong> ' . htmlspecialchars($value) . '</p>';
                            }
                        }
                    } else {
                        echo '<p>Aucune donnée reçue via GET</p>';
                    }
                    
                    echo '<p><strong>URL actuelle:</strong> ' . htmlspecialchars($_SERVER['REQUEST_URI']) . '</p>';
                    echo '</div>';
                }
                ?>
            </div>
            
            <!-- Section Méthode POST -->
            <div class="section">
                <h2>Méthode POST <span class="method-badge post-badge">POST</span></h2>
                <p><strong>Caractéristiques :</strong></p>
                <ul style="margin-left: 20px; margin-bottom: 15px;">
                    <li>Données cachées (non visibles dans l'URL)</li>
                    <li>Pas de limite de taille pratique</li>
                    <li>Plus sécurisé pour les données sensibles</li>
                    <li>Idéal pour les formulaires d'inscription</li>
                </ul>
                
                <form method="post" action="">
                    <div class="form-group">
                        <label for="post_name">Nom:</label>
                        <input type="text" id="post_name" name="post_name" value="<?php echo isset($_POST['post_name']) ? htmlspecialchars($_POST['post_name']) : ''; ?>" placeholder="Votre nom" required>
                    </div>
                    
                    <div class="form-group">
                        <label for="post_email">Email:</label>
                        <input type="email" id="post_email" name="post_email" value="<?php echo isset($_POST['post_email']) ? htmlspecialchars($_POST['post_email']) : ''; ?>" placeholder="votre@email.com" required>
                    </div>
                    
                    <div class="form-group">
                        <label for="post_password">Mot de passe:</label>
                        <input type="password" id="post_password" name="post_password" placeholder="Votre mot de passe" required>
                    </div>
                    
                    <div class="form-group">
                        <label for="post_message">Message:</label>
                        <textarea id="post_message" name="post_message" rows="4" placeholder="Votre message..."><?php echo isset($_POST['post_message']) ? htmlspecialchars($_POST['post_message']) : ''; ?></textarea>
                    </div>
                    
                    <button type="submit" class="post-btn" name="submit_post">Envoyer avec POST</button>
                </form>
                
                <?php
                if ($_SERVER['REQUEST_METHOD'] == 'POST' && isset($_POST['submit_post'])) {
                    echo '<div class="result post-result">';
                    echo '<h3>Données reçues via POST:</h3>';
                    
                    if (!empty($_POST)) {
                        foreach ($_POST as $key => $value) {
                            if ($key !== 'submit_post') {
                                if ($key === 'post_password') {
                                    echo '<p><strong>' . htmlspecialchars($key) . ':</strong> ********</p>';
                                } else {
                                    echo '<p><strong>' . htmlspecialchars($key) . ':</strong> ' . htmlspecialchars($value) . '</p>';
                                }
                            }
                        }
                    } else {
                        echo '<p>Aucune donnée reçue via POST</p>';
                    }
                    
                    echo '<p><strong>URL actuelle:</strong> ' . htmlspecialchars($_SERVER['PHP_SELF']) . '</p>';
                    echo '</div>';
                }
                ?>
            </div>
            
            <!-- Section Comparaison et Informations -->
            <div class="section">
                <h2>Comparaison GET/POST</h2>
                
                <div class="data-display">
                    <h3>Données Superglobales</h3>
                    
                    <div class="form-group">
                        <h4>$_GET</h4>
                        <pre style="background: white; padding: 10px; border-radius: 4px; overflow-x: auto;"><?php print_r($_GET); ?></pre>
                    </div>
                    
                    <div class="form-group">
                        <h4>$_POST</h4>
                        <pre style="background: white; padding: 10px; border-radius: 4px; overflow-x: auto;"><?php print_r($_POST); ?></pre>
                    </div>
                    
                    <div class="form-group">
                        <h4>$_REQUEST (GET + POST)</h4>
                        <pre style="background: white; padding: 10px; border-radius: 4px; overflow-x: auto;"><?php print_r($_REQUEST); ?></pre>
                    </div>
                </div>
                
                <h3 style="margin-top: 20px;">Informations Serveur</h3>
                <div class="info-item">
                    <strong>Méthode HTTP utilisée:</strong> <?php echo $_SERVER['REQUEST_METHOD']; ?>
                </div>
                <div class="info-item">
                    <strong>Script actuel:</strong> <?php echo $_SERVER['PHP_SELF']; ?>
                </div>
                <div class="info-item">
                    <strong>URL complète:</strong> <?php echo $_SERVER['REQUEST_URI']; ?>
                </div>
                
                <h3 style="margin-top: 20px;">Test Mixte GET/POST</h3>
                <form method="post" action="?action=mixed&source=form_mixte">
                    <div class="form-group">
                        <label for="mixed_data">Donnée mixte:</label>
                        <input type="text" id="mixed_data" name="mixed_data" placeholder="Ceci sera envoyé en POST vers une URL GET">
                    </div>
                    <button type="submit" name="submit_mixed">Tester GET + POST</button>
                </form>
                
                <?php
                if (isset($_POST['submit_mixed'])) {
                    echo '<div class="result" style="border-left-color: purple;">';
                    echo '<h3>Résultat Mixte GET/POST:</h3>';
                    echo '<p><strong>Paramètres GET (URL):</strong></p>';
                    echo '<ul>';
                    foreach ($_GET as $key => $value) {
                        echo '<li><strong>' . htmlspecialchars($key) . ':</strong> ' . htmlspecialchars($value) . '</li>';
                    }
                    echo '</ul>';
                    
                    echo '<p><strong>Paramètres POST (Formulaire):</strong></p>';
                    echo '<ul>';
                    foreach ($_POST as $key => $value) {
                        if ($key !== 'submit_mixed') {
                            echo '<li><strong>' . htmlspecialchars($key) . ':</strong> ' . htmlspecialchars($value) . '</li>';
                        }
                    }
                    echo '</ul>';
                    echo '</div>';
                }
                ?>
            </div>
        </div>
        
        <footer>
            <p>Page de test méthodes GET et POST - PHP <?php echo phpversion(); ?> - <?php echo date('Y'); ?></p>
        </footer>
    </div>
</body>
</html>