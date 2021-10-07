import pygame
import settings
import globalVar
import os
from math import *
from random import random
from sprite import *

def sumArray(a, b, c = (0,0)):
    return [ a[0]+b[0]+c[0], a[1]+b[1]+c[1] ]

def mulArray(a, b, c = (1,1)):
    return [ a[0]*b[0]*c[0], a[1]*b[1]*c[1] ]

def reverseArray(t):
    return [-t[0], -t[1]]

def angleVec(posDest, posO = (0,0)):
    """ Retourne l'angle entre l'axe des absices et du vecteur(posO,posDest) """
    vec = (posDest[0] - posO[0], posDest[1] - posO[1])
    if vec[0] == 0 and vec[1] == 0 : return 0
    if vec[0] == 0:
        return pi/2 * sign(vec[1])
    else:
        return atan(vec[1] / vec[0]) + pi * (vec[0] < 0)

def sign(x):
    """ Retourne le signe d'une variable (-1 ou 1)"""
    if x == 0: return 0
    return int(x / abs(x))

def dist(posA, posB):
    return sqrt( (posA[0]-posB[0])**2 + (posA[1]-posB[1])**2 )

class Rect:
    """ Classe qui comporte la position, et taille d'un objet rectangulaire, 
    similairement a pygame.Rect, sauf qu'il peut avoir des coordonnées flottants"""
    def __init__(self, pos = (0,0), size = (0,0)):
        self.pos = list(pos)
        self.size = size

    # Retourne l'absice des limites du rectangle
    def left(self): return self.pos[0]
    def right(self): return self.pos[0]+self.size[0]
    # Retourne l'ordonnée des limites du rectangle
    def top(self): return self.pos[1]
    def bottom(self): return self.pos[1]+self.size[1]
    # Retourne les coordonnées du centre du rectangle
    def center(self): return (self.pos[0] + self.size[0]/2, self.pos[1] + self.size[1]/2)

    # Retourne la version pygame.Rect de cet objet, pour pouvoir l'utiliser avec les fonctions de pygame 
    def pgRect(self): return pygame.Rect(self.pos, self.size)

def shareIntervalOnX(rect1 = Rect(), rect2 = Rect()):
    """ Vérifie si les deux rectangles ont des intervalles [left;right] qui partagent une portion"""
    return rect1.left() < rect2.right() and rect1.right() > rect2.left()

class Camera:
    def __init__(self):
        self.rect = pygame.Rect( [0,0], settings.windowSize )

    def posRelativeToCamera(self, rectObjet):
        """ Position relative d'un rectangle par rapport à celle de la caméra, ce qui permet de changer """
        return Rect( sumArray(rectObjet.pos, reverseArray(self.rect.topleft) ) , rectObjet.size )

cam = Camera()

class Platform:
    i = 0 # Variable statique qui permet de numéroter les plateformes lors de leurs créations 
    def __init__(self, width = 100, pos = (0,0)):
        self.rect = Rect(pos, (width,10)) # Hitbox, avec une hauteur constante
        self.spr = makeRectSpr(self.rect.size, (0,255,0)) 
        self.number = Platform.i # Numéro de la plateforme
        Platform.i += 1 # Incrément de la variable statique, qui sera le numéro de la prochaine plateforme créée

    def draw(self):
        SCREEN.blit(self.spr.getUsedSurf(), cam.posRelativeToCamera(self.rect).pgRect() )


class Bullet:
    def __init__(self, pos, velocity, dmg = 10):
        self.rect = Rect(pos, (5,5)) # Hitbox
        self.velocity = velocity # Vecteur vitesse
        self.spr = sprBullet
        self.dmg = dmg # Dégats infligés

    def update(self):
        """Déplacement de la balle en fonction de velocity"""
        self.rect.pos = sumArray(self.rect.pos, self.velocity)

    def draw(self):
        self.spr.cycle() # Change la texture du sprite en fonction du temps
        SCREEN.blit(self.spr.getUsedSurf(),  cam.posRelativeToCamera(self.rect).pgRect())

class Gun:
    """ Classe qui génère des objets Bullet en fonction des paramètres"""
    def __init__(self, spd = 20, nBullet = 1, spread = radians(5), dmg = 10, reloadTime = 10): 
        self.spd = spd # Vitesse de la balle
        self.nBullet = nBullet # Nombre de balles tirés à la fois
        self.spread = spread # Ecart angulaire des balles (radians)
        self.dmg = dmg # Dégats infligés par balle
        self.spr = makeRectSpr((30,10), (191,64,128))
        self.reloadTime = reloadTime # Nombre d'images qui doivent passer avant de recharger
        self.lastShot = 0 # Numéro de l'image au dernier tir

    def shoot(self,pos,alpha): 
        """ Retourne une liste d'objets Bullet de position pos (celle du joueur), 
        et qui vont vers la direction alpha (en radians)
        Retourne une liste vide si le temps de rechargement n'est pas écoulée """
        r = []
        if globalVar.FRAME-self.lastShot >= self.reloadTime: 
            for i in range(self.nBullet):
                a = alpha + self.spread * (random() - 0.5) # Direction plus au moins aléatoire en fonction de l'écart angulaire
                r.append(Bullet(pos, (cos(a)*self.spd, sin(a)*self.spd), self.dmg) )
            self.lastShot = globalVar.FRAME 
        return r

# Création d'armes pour le jeu
guns = [Gun()]                                                                             # Pistolet
guns.append( Gun(spd = 60, nBullet =  1, spread = radians(0), dmg = 35, reloadTime = 60))  # Fusil de chasse
guns.append( Gun(spd = 30, nBullet =  1, spread = radians(20), dmg = 10, reloadTime = 5))  # Fusil d'assaut
guns.append( Gun(spd = 10, nBullet =  7, spread = radians(30), dmg = 10, reloadTime = 30)) # Fusil à pompe

class Shooter:
    """Personnage jouable, """
    def __init__(self, pos = (0,0)):
        self.rect = Rect(pos, (20,64)) # Hitbox
        self.rectDraw = Rect(pos, (64,64)) # Rectangle de la destionation du dessin
        self.velocity = [0,0] # Vecteur vitesse
        self.spr = sprPlayer
        self.rectCursor = Rect((0,0), ((5,5))) # Position du curseur relative au joueur
        self.rectCursorDraw = Rect((0,0), (5,5)) # Position du curseur relative au monde
        self.sprCur = makeRectSpr(self.rectCursorDraw.size, (255,255,255))
        self.onPlatform = False # Vrai si le joueur est sur une plateforme
        self.platOn = 0 # Numéro de la plateforme que le joueur est au-dessus
        self.health = 100 
        self.arsenal = guns # Liste d'objets Gun
        self.equiped = 0 # Arme utilisée

    def draw(self):
        self.rectCursorDraw.pos = sumArray(self.rectCursor.pos, self.rect.center()) 

        # On place le centre de la caméra entre le joueur et le curseur
        temp = sumArray( self.rectCursorDraw.pos, self.rect.center() )
        cam.rect.center = (temp[0]/2 , temp[1]/2) 
        # La position de la texture est décalée par rapport au hitbox, donc on la modifie
        self.rectDraw.pos = sumArray(self.rect.pos, (-(64-20)/2 , 0) ) 

        SCREEN.blit(self.spr.getUsedSurf(), cam.posRelativeToCamera(self.rectDraw).pgRect())
        SCREEN.blit(self.sprCur.getUsedSurf(), cam.posRelativeToCamera(self.rectCursorDraw).pgRect())
        self.spr.cycle()

        # Changement de texture en fonction de l'état du joueur
        direction = sign(self.velocity[0])
        if direction == 0: direction = 1
        if not self.onPlatform: self.spr.changeState( 2*direction )
        elif self.velocity[0] != 0: self.spr.changeState( 1*direction )
        else: self.spr.changeState(0)

    def update(self):
        """ Changement de position en fonction du vecteur vitesse, et accéleration de la chute """
        if self.isDead(): self.velocity[0] = 0 # Arrêt du mouvement horizontal à la mort
        self.rect.pos = sumArray(self.velocity, self.rect.pos) 
        if not(self.onPlatform): self.velocity[1] += 0.5 # Chute
        self.velocity[1] = min(50, self.velocity[1]) # On définit la vitesse de chute maximale

    def jump(self):
        # Interompt la fonction si le joueur n'est pas sur une plateforme, ou si il est mort
        if self.isDead() or not self.onPlatform: return 
        self.velocity[1] = -10
        self.onPlatform = False # Le joueur n'st plus sur une plateforme
    def move(self, d):
        """ Déplacement horizontal"""
        if self.isDead(): return 
        if abs(d) > 1: d = sign(d) # d = 1 ou -1
        self.velocity[0] = d * 7
    def dropDown(self):
        """ Descends de la plateforme"""
        if self.isDead(): return
        self.rect.pos[1] += 1 # On évite de le laisser sur la plateforme dans la prochaine image
        self.startFalling()

    def startFalling(self):
        """ Activation de la chute"""
        self.onPlatform = False 
    def stopFalling(self, y, nPlat):
        """ Arret de la chute sur la position d'une plateforme """
        self.onPlatform = True
        self.rect.pos[1] = y-self.rect.size[1]
        self.velocity[1] = 0
        self.platOn = nPlat # Attribution du numéro de la plateforme

    def takeDmg(self, dmg):
        """ Perds de la santé """
        self.health -= dmg
        self.health = max(self.health, 0) # On définit la santé minimale

    def isDead(self):
        return self.health <= 0

    def moveCursor(self, movement = (0,0)):
        """ Déplacement du curseur """
        self.rectCursor.pos = sumArray( (movement[0] * settings.cursorSpeed, movement[1] * settings.cursorSpeed) , self.rectCursor.pos)
        # On empêche le curseur de sortir de l'écran
        self.rectCursor.pos[0] = max(-settings.windowSize[0] , self.rectCursor.pos[0])
        self.rectCursor.pos[0] = min( settings.windowSize[0] , self.rectCursor.pos[0])
        self.rectCursor.pos[1] = max(-settings.windowSize[1] , self.rectCursor.pos[1])
        self.rectCursor.pos[1] = min( settings.windowSize[1] , self.rectCursor.pos[1])
    def shoot(self):
        """ Retourne une liste d'objets Bullet, que le joueur tire avec l'arme équipée"""
        if self.isDead(): return []
        alpha = angleVec(self.rectCursor.center()) # Angle entre l"axe des absices, et l'axe entre le joueur et le curseur
        return self.arsenal[self.equiped].shoot(self.rect.center(), alpha) # Création des balles à la position du joueur

class Enemy: 
    """ Classe qui reptrésente un bloc se délpaçant sur une plateforme, et qui tire sur le joueur si il est assez proche"""
    def __init__(self, assignedPlat, spd = 5, size = 20, atkRange = 500):
        self.rect = Rect( sumArray(assignedPlat.rect.pos, (0, -size) ), (size,size) ) # Hitbox
        self.space = (assignedPlat.rect.left(), assignedPlat.rect.right()) # L'intervalle où il se déplace
        self.spr = makeRectSpr(self.rect.size, (255,0,0))
        self.spd = spd # Vitesse
        self.health = 40 
        self.direction = 1 
        self.gun = Gun() 
        self.range = atkRange # Portée des tirs
    def update(self):
        """ Déplacement horizontal"""
        if self.isDead(): return # Interruption de la fonction
        self.rect.pos[0] += self.direction * self.spd
        # Changement de direction quand il sort de son espace
        if self.rect.right() >= self.space[1] or self.rect.left() <= self.space[0]: self.direction = -self.direction
    def draw(self):
        SCREEN.blit(self.spr.getUsedSurf(), cam.posRelativeToCamera(self.rect).pgRect() )
    def shoot(self, playerPos):
        """ Retourne une liste d'objets Bullet"""
        if dist(self.rect.pos, playerPos) > self.range or self.isDead(): return []
        alpha = angleVec(playerPos, self.rect.center())
        return self.gun.shoot(self.rect.center(), alpha)
    def takeDmg(self, dmg):
        self.health -= dmg
    def isDead(self):
        return self.health <= 0

class EndLevel:
    """ Classe qui représente l'objectif à atteindre chaque niveau"""
    def __init__(self, assignedPlat):
        self.rect = Rect( (assignedPlat.rect.center()[0], assignedPlat.rect.top()-100), (20,100) ) # Hitbox, au centre du niveau
        self.spr = makeRectSpr( self.rect.size, (0,0,255) )
    def draw(self):
        SCREEN.blit( self.spr.getUsedSurf(), cam.posRelativeToCamera(self.rect).pgRect() )

def pause():
    global PAUSE
    PAUSE = not(PAUSE) # Active ou désactive pause

def getInput(pause_ = False):
    """ Applique les méthodes de l'objet joueur et du jeu en fonction de l'entrée
    si le jeu est en pause, on désactive certaines entrées"""
    global player, QUIT, bulletsP
    for e in pygame.event.get():
        if e.type == pygame.QUIT: QUIT = True
        # Détection de touche préssée
        if e.type == pygame.KEYDOWN: 
            if e.key == pygame.K_p: pause()
            if e.key == pygame.K_ESCAPE: QUIT = True # Quitte le jeu
            if not pause_:
                if e.key == pygame.K_r: loadLevel()
                if e.key == pygame.K_w: player.equiped = 0
                if e.key == pygame.K_x: player.equiped = 1
                if e.key == pygame.K_c: player.equiped = 2
                if e.key == pygame.K_v: player.equiped = 3

    # Détection de touche maintenue
    e = pygame.key.get_pressed()
    if not pause_:
        if e[pygame.K_z]: player.jump() 
        if e[pygame.K_q]: player.move(-1)
        elif e[pygame.K_d]: player.move(1)
        else: player.move(0)
        if e[pygame.K_s]: player.dropDown()
        if pygame.mouse.get_pressed()[0]: bulletsP += player.shoot()
        player.moveCursor( pygame.mouse.get_rel() )
    # pygame.mouse.get_rel() retourne le déplacement de la souris depuis le dernier appel de cette fonction,
    # donc on peut y faire appel pour ignorer les déplacements de la souris pendant la pause
    pygame.mouse.get_rel()

def printText(text):
    """ Retourne un objet Animation, qui se détruit après 3 secondes, et quui a pour texture du texte"""
    font = pygame.font.SysFont("Consolas", 50)
    surf = font.render(text, True ,(255,255,255))
    anim = Animation([surf, 0], refreshPeriod = 240 ,destroyAtEnd = True)
    anim.start() 
    return anim

def loadLevel(file = ""):
    """ Charge le niveau depuis un fichier texte. Le format du fichier est expliqué dans levelFormat.txt  """
    global player, platforms, enemies, finish, bulletsP, bulletsE, levelId, levelsFile
    # Si le nom du fichier n'est pas indiqué, on charge le niveau de la liste levelsFile
    if(file == ""): file = settings.levelFolder + str(levelsFile[levelId]) 
    Platform.i = 0 # On réinitialise la numérotation des plateformes, avec la variable statique i
    F = open(file) # Lecteur du fichier

    shterPos = list( map( int , F.readline().split() ) ) # On divise la chaine de caractere, pour obtenir plusieurs valeurs
    player = Shooter(shterPos) 
    n = int(F.readline()) 
    platforms = []
    for i in range(n):
        t = list( map( int , F.readline().split() ) )
        width, pos = t[0] , (t[1], t[2])
        platforms.append( Platform(width, pos) ) 
    enemies = []
    n = int(F.readline())
    for i in range(n):
        nPlat, spd, size, atkRange = list( map( int , F.readline().split() ) )
        enemies.append( Enemy( platforms[nPlat], spd, size, atkRange ) )
    finish = EndLevel(platforms[ int(F.readline()) ])
    bulletsP = []; bulletsE = [] # On supprime toutes les balles qui ont étaient tirées

pygame.init()
pygame.mouse.set_visible(False) # On cache la souris
QUIT = False
PAUSE = False
SCREEN = pygame.display.set_mode( settings.windowSize, pygame.FULLSCREEN * settings.fullscreen )

# Création d'un niveau alternativement sans la fonction loadLevel
##player = Shooter((201, 0))                                                                # Personnage controlé par le joueur
##platforms = [Platform(100, (0, 200)), Platform(200, (0, 500)), Platform(50, (500, 500))]  # Liste des plateformes
##enemies = [Enemy(platforms[2])]                                                           # Liste des ennemis
##bulletsE = []                                                                             # Liste des balles tirées par l'ennemi
##bulletsP = []                                                                             # Liste des balles tirées par le joueur

levelsFile = os.listdir( settings.levelFolder ) # Liste des fichiers du dossier des niveaux
levelId = 0 # Progression dans les niveaux
loadLevel() 
print(levelsFile)

txt = [printText("Level " + str(levelId + 1)) ] # Liste contenant les textures de textes à afficher

# Boucle du jeu
while not QUIT:

    pygame.time.Clock().tick( settings.ips ) # Fonction permettant de rafraichir le jeu IPS fois par secondes
    getInput(PAUSE) 

    p = platforms # Définition d'un macro, pour simplifier l'écriture

        # ||Partie graphique||

    SCREEN.fill((0,0,0))
    player.draw()
    finish.draw()
    for i in range(len(p)):
        p[i].draw()
    for i in range(len(enemies)):
        enemies[i].draw()
    for i in range(len(bulletsE)):
        bulletsE[i].draw()
    for i in range(len(bulletsP)):
        bulletsP[i].draw()
    
    # On parcoure la liste à l'envers pour éviter des erreurs d'index après la supression d'un élément
    for i in range(len(txt)-1,0-1,-1): 
        if txt[i].destroyAtEnd and txt[i].isOver(): 
            del txt[i] # Supprime la texture d'un texte si sa durée est finie
            continue
        SCREEN.blit(txt[i].getUsedSurf(), (0,0)) # On affiche le texte en haut à gauche
        txt[i].cycle()

    pygame.display.flip()

    if(PAUSE): continue # On recommence la boucle

        # ||Partie logique||

    # Déplacement du joueur
    player.update()
    # Limitation des balles dans le jeu
    while len(bulletsE) > 100: del(bulletsE[0])
    while len(bulletsP) > 100: del(bulletsP[0])

    # Déplacement des balles des enemies, et détection de collision avec le joueur
    for i in range(len(bulletsE)-1,0-1,-1):
        bulletsE[i].update()
        if  bulletsE[i].rect.pgRect().colliderect(player.rect.pgRect()): 
            player.takeDmg(bulletsE[i].dmg)
            del bulletsE[i]

    # Déplacement des balles du joueur, et détection de collision avec les enemies
    for i in range(len(bulletsP)): 
        bulletsP[i].update()

    for i in range(len(enemies)):
        for j in range(len(bulletsP)-1, 0-1, -1):
            if bulletsP[j].rect.pgRect().colliderect(enemies[i].rect.pgRect()):
                enemies[i].takeDmg(bulletsP[j].dmg)
                del bulletsP[j]

    # Détecte si le joueur est sur une plateforme, et controle la chute
    if not player.onPlatform: 
        for i in range(len(p)):
            # Cette condition vérifie si le joueur va traverser une plateforme dans la prochaine image en fonctio de sa vitesse
            if shareIntervalOnX(player.rect, p[i].rect) and player.velocity[1] > p[i].rect.top()-player.rect.bottom() and player.rect.bottom() <= p[i].rect.top():
                player.stopFalling( p[i].rect.top() , p[i].number) # Arrêt du joueur sur la plateforme
                break
    elif not shareIntervalOnX(player.rect, platforms[player.platOn].rect): player.startFalling()

    # Le joueur atteint la fin du niveau
    if player.rect.pgRect().colliderect(finish.rect.pgRect()): 
        if levelId >= len(levelsFile)-1: txt = [ printText("Game over, you did great!")] # Affichage du texte si il ne reste plus de niveau
        # Sinon, le prochain niveau est chargé
        else:
            levelId += 1
            loadLevel()
            txt.append(printText("Level " + str(levelId+1) )) 

    # Attaque des enemies
    d = []
    for i in range(len(enemies)-1, 0-1, -1):
        enemies[i].update()
        if not player.isDead(): 
            bulletsE += enemies[i].shoot(player.rect.center())
        if enemies[i].isDead(): del enemies[i]

    globalVar.FRAME += 1

pygame.quit()