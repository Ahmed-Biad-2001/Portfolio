-- A PROPOS --
Ceci est un jeu de Morpion sur réseau réalisé en binome dans le cadre du projet informatique à l'IUT de Saint-Étienne en 2021.

C'est une application Windows Forms, codé en en C# avec le framework .NET, qui donne la possiblité de jouer avec quelqu'un d'autre en réseau local.

-- COMPILATION -- 
Le projet a été réalisé sous Visual Studio Community, qui est nécessaire pour la compilation, ainsi que les modules de C#, de .NET, et Windows Forms.
Il suffit d'ouvrir le fichier du solution (.sln) et d'appyer sur F5 pour génerer l'éxecutable et le lancer.

-- EXECUTION --
L'executable se trouve sous Morpion/bin/Release/Morpion.exe
Il est possible de jouer sur la même machine.

Usage minimal:
    - Lancer Morpion.exe 2 fois, un pour le serveur et un pour le client.
    - Pour le serveur, saisir un caractère pour le nom, cocher la case "je suis serveur", et cliquer sur "Démarrer".
    - Pour le client,  saisir un caractère pour le nom, décocher la case "je suis serveur", laisser "localhost" comme adresse IP, et cliquer sur "Démarrer".
    - Les deux joueurs peuvent maintenant jouer tour à tour.
    
