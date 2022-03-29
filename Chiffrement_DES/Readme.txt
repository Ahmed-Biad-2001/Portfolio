-- A PROPOS --

Ceci est une application console codé en C dans le cadre du projet TIPE sur la cryptographie. 

Il s'agit d'une implémentation du chiffrement Data Encryption Standard, qui est une technique de chiffrement symétrique (càd on utilise la même clé pour le cryptage et le décryptage). Plus d'infos sur la page Wikipedia: https://fr.wikipedia.org/wiki/Data_Encryption_Standard 

L'executable peut faire différents tests par ligne de commade.


-- COMPILATION -- 

Le projet a été réalisé dans un environnement Linux, et peut nécessiter des étapes en plus pour le faire fonctionner sur votre machine

Dépendances:
- gcc

Géneration de l'éxecutable:
    $ gcc chiffrementDES.c -o chiffrementDES
    
-- EXECUTION --

Usage:
    $ ./chiffrementDES [choix] [in] [key]

    choix: entier entre 1 et 4 (par défaut 1)
    in et key: valeurs hexadecimales de 64 bits (ont des valeurs par défaut)

    - choix=1 :  Test d'un tour de Feistel (in et key sont ignorés)
    - choix=2 :  Test de la génération des sous-clés à partir de la clé (in est ignorée)
    - choix=3 :  Test de cryptage et de décryptage. 
                 Les résultats de ce test peuvent être comparés dans le site web http://des.online-domain-tools.com/
    - choix=4 :  Test de performance (in et key sont ignorés)
    
Exemples d'usage:
    
    $ ./chiffrementDES 4
    Choix 4
    Calcul de la vitesse de chiffrement
    64kb en 0.027558 secondes
    290.296814 MO/s
    
    $ ./chiffrementDES 3 898acd707a707961 123878abc21346ef
    Choix 3
    Test du chiffrement
    --CIPHER--
    in:             898ACD707A707961
    key:            123878ABC21346EF
    out_cipher:     1ADC77C222904C3E
    --DECIPHER--
    out_decipher:   898ACD707A707961
    
    
