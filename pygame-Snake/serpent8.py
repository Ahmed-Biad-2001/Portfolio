# Jeu du Serpent en utilisant les fonctions de la librairie Pygame

import pygame
from random import randint
pygame.init()

# Initialisation des variables
fontSize = 18
tailleGrille = (10,10) # De 0 à 29
wallWidth = 10
pas = 10 # Longueur du déplacement du cercle

gridWidth = tailleGrille[0]*pas
gridHeight = tailleGrille[1]*pas
winWidth = gridWidth + 2*wallWidth
winHeight = gridHeight + 2*wallWidth + fontSize

fenetre = pygame.display.set_mode( (winWidth ,winHeight) ) # Définition de la taille du terrain de jeu
pygame.display.set_caption("BIAD Ahmed TS1 - SNAKE")

textFont = pygame.font.SysFont("Consolas", fontSize)
textSurf = pygame.Surface((winWidth,fontSize))

ground = pygame.Surface((tailleGrille[0]*pas,tailleGrille[1]*pas))
ground.fill(pygame.Color(0,255,0))
#appleSurf = pygame.image.load("apple.bmp")
def getPixCoordsRect(gridCoords):
    return (gridCoords[0]*pas + wallWidth , gridCoords[1]*pas + wallWidth)
def getPixCoords(gridCoords):
    return (gridCoords[0]*pas + int(pas/2) + wallWidth , gridCoords[1]*pas + int(pas/2) + wallWidth)
apple = (0,0)
serpent = [ (0,0) ]
direction ="sud"

def directionInverse(direction):
    if direction == "nord":
        return "sud"
    if direction == "sud":
        return "nord"
    if direction == "est":
        return "ouest"
    if direction == "ouest":
        return "est"

def dessiner(apple, serpent, fenetre): # Procédure d'affichage du serpent
    #global serpent, fenetre
    fenetre.fill( (255,0,255) )
    #ground.blit(fenetre,(wallWidth,wallWidth))
    pygame.draw.rect(fenetre, (0,255,0), (wallWidth,wallWidth, tailleGrille[0]*pas, tailleGrille[1]*pas))
    for i in range(1,len(serpent)):
        position = serpent[i]
        pygame.draw.circle(fenetre, (0,200,0), getPixCoords(position), int(pas/2)) # dessine le cercle de centre position et de rayon 5
    pygame.draw.circle(fenetre, (150,150,0), getPixCoords(serpent[0]), int(pas/2))
    pygame.draw.circle(fenetre, (200,0,0), getPixCoords(apple), int(pas/2))
    #fenetre.blit(appleSurf, getPixCoordsRect(apple))
    fenetre.blit(textSurf,(0, gridWidth + wallWidth*2))
    pygame.display.flip()
    return serpent, fenetre

def spawnApple():
    return (randint(0, tailleGrille[0]-1), randint(0, tailleGrille[1]-1))

def gererClavierEtSouris(direction, continuer): # Procédure de Détection de la frappe clavier du joueur
    #global direction, continuer
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            continuer = False
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_RIGHT:
                direction = "est"
            if event.key == pygame.K_LEFT:
                direction = "ouest"
            if event.key == pygame.K_UP:
                direction = "nord"
            if event.key == pygame.K_DOWN:
                direction = "sud"

    return direction, continuer

perdu = False
continuer = True # Déclaration de la variable
apple = spawnApple()
IPS = 3
score = 0
while continuer: # Cette boucle s'exécute jusqu'à ce que le joueur ferme la fenêtre
    pygame.time.Clock().tick(IPS+score)
    serpent, fenetre = dessiner(apple, serpent, fenetre)
    directionNew, continuer=gererClavierEtSouris(direction, continuer)
    if directionInverse(directionNew) != direction:
        direction = directionNew
    dernierePosition = serpent[0] # Mémorisation de la dernière position du cercle (abscisse,ordonnée)
    if not(perdu): # arrête de bouger quand perdu = True
        if direction == "nord":
            serpent.insert(0, (dernierePosition[0], dernierePosition[1] - 1) ) # 0 pour la première position du tableau et donnée de la nouvelle valeur de la position du serpent.
        elif direction == "sud":
            serpent.insert(0, (dernierePosition[0], dernierePosition[1] + 1) )
        elif direction == "est":
            serpent.insert(0, (dernierePosition[0] + 1, dernierePosition[1]) )
        elif direction == "ouest":
            serpent.insert(0, (dernierePosition[0] - 1, dernierePosition[1]) )
        textSurf = textFont.render("Score: " + str(score), True, (0,255,0))
        serpent.pop(len(serpent)-1)

    if serpent[0]==apple:
        serpent.append(dernierePosition)
        score += 1
        i = 0
        apple=spawnApple()
        while apple in serpent: # pomme cherche une place vide
            i+=1
            print(i)
            if(i==1000):
                perdu = True
                break
            apple=spawnApple()
    if serpent[0][0]<0 or serpent[0][0]>=tailleGrille[0] or serpent[0][1]<0 or serpent[0][1]>=tailleGrille[1]: # sortie de la grille?
        perdu = True
    for i in range(1, len(serpent)):
        if(serpent[0] == serpent[i]):
            perdu = True
    if perdu:
        textSurf = textFont.render("PERDU - Score final: " + str(score), True, (0,255,0))
pygame.quit()
