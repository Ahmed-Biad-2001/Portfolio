-- A PROPOS --
Ceci est une application réalisée durant un stage au sein du laboratoire Hubert-Curien en 2021. 

Elle permet de faire une simulation d'un microscope à tomographie diffractive, qui s'agit d'une technique de microscopie qui permet l'acquisition d'hologrammes 2D à partir d'un échantillon 3D, cet échantillon pouvant être une simple bille sphérique, un grain de pollen, ou même une bactérie. La simulation est une partie vitale pour la reconstruction numérique de l'hologramme, et ma mission concernait  la prise en connaissance de l'algorithme de simulation écrite en matlab, et de le réecrire C++ avec la bibliothèque OpenCV. 

-- COMPILATION -- 
Le projet a été réalisé dans un environnement Linux, et peut nécessiter des étapes en plus pour le faire fonctionner sur votre machine

Dépendances:
- g++
- Bibliothèque opencv pour C++

Géneration de l'éxecutable:
    $ chmod +x Compile.sh
    $ ./Compile.sh
    
-- EXECUTION --
L'executable prend des paramètres en ligne de commande, et génère 2 fichiers dans "bin/Test" images représentant l'amplitude et la phase des ondes lumineuses traversant un échantillon sphérique.

Il existe déjà dans ce dossier différents tests qui font varier les paramètres phi et theta.

Usage:
    $ cd bin
    $ Simulation nom_test z thetad phid r_bead lambda n_objet n_0 nb_p dpix NA
    
    (Tous les paramètres sont optionnels, mais pour en saisir un il faut saisir tous les paramètres précédents ce qui précède)    
    nom_test :  nom du test, qui sera généré dans 'bin/Test'(Defaut: 'out')
    z :         Profondeur de propagation en mètres         (Defaut: 10e-6)
    thetad :    Angle theta des ondes incidents en degrées  (Defaut: 0)
    phid :      Angle phi des ondes incidents en degrées    (Defaut: 0)
    r_bead :    Rayon de la sphère en mètres                (Defaut: 2.5e-6)
    lambda :    Longueur d'onde en mètres                   (Defaut: 632e-9)
    n_objet :   Indice de réfraction de l'objet             (Defaut: 1.45)
    n_0 :       Indice de réfraction du milieu              (Defaut: 1.5190)
    nb_p :      Nombre de pixels au bord de l'image génerée (Defaut: 128)
    dpix :      Mètres par pixel                            (Defaut: 4.51428e-07)
    NA :        Ouverture numérique                         (Defaut: 0.9)

