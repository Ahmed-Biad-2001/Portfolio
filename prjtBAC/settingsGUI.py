from tkinter import *
from shutil import copyfile

def quit():
    global WINDOW
    WINDOW.destroy()

def save():
    global entryResWidth, entryResHeight, entryIps, fullscreen, scaleCurSpeed, entryLvl
    width = int(entryResWidth.get()); height=int(entryResHeight.get())
    ips = entryIps.get()
    f = str(bool(fullscreen.get()))
    spd = scaleCurSpeed.get()
    lvl = entryLvl.get()
    if len(lvl) == 0: lvl = 'Levels/'
    if lvl[-1] != '/': lvl += '/'
    settingsFile = open("settings.py",'w')
    settingsFile.write("windowSize = " + str((width,height)) + '\n')
    settingsFile.write("ips = " + str(ips) + '\n')
    settingsFile.write("fullscreen = " + f + '\n')
    settingsFile.write("cursorSpeed = " + str(spd/100) + '\n')
    settingsFile.write("levelFolder = " + "'" + lvl + "'"  + '\n')
    quit()

def defaults():
    copyfile("_defaultSettings.py","settings.py")
    quit()

WINDOW = Tk()
labelGfx = Label(WINDOW, text="Graphismes"); labelGfx.grid(row=0)
frameGfx = Frame(WINDOW); frameGfx.grid(row=1,padx=5, pady=5)   # Resolution(Spinbox*2), AutoRes(Button), Fullscreen(checkox), IPS(Entry), Use system resolution(Button)
labelMisc = Label(WINDOW, text="Divers"); labelMisc.grid(row=2)
frameMisc = Frame(WINDOW); frameMisc.grid(row=3,padx=5, pady=5) # LevelFolder(Entry), cursorSpeed(Scale)
frameAction = Frame(WINDOW); frameAction.grid(row=4,padx=5, pady=5) # Save(Button), ResetDef(Button), Cancel(Button)
labelMsg = Label(WINDOW, text="Ce programme modifie le fichier settings.py"); labelMsg.grid(row=5,padx=5, pady=5)

# Graphismes
labelWidth = Label(frameGfx, text="Longueur") ; labelWidth.grid(row = 1, column = 1)
entryResWidth = Spinbox(frameGfx, from_=400, to=5000, width = 4) ; entryResWidth.grid(row = 1, column = 2, padx = 5)
labelHeight = Label(frameGfx, text="Hauteur") ; labelHeight.grid(row = 1, column = 3)
entryResHeight = Spinbox(frameGfx, from_=400, to=5000, width = 4) ; entryResHeight.grid(row = 1, column = 4, padx = 5)

labelIps = Label(frameGfx, text="Images par seconde") ; labelIps.grid(row = 2, column = 1, columnspan = 4)
entryIps = Spinbox(frameGfx, from_=0, to=60, width = 2) ; entryIps.grid(row = 2, column = 4)
fullscreen = IntVar()
chBoxFullscreen = Checkbutton(frameGfx, text="Plein écran", variable=fullscreen); chBoxFullscreen.grid(row = 3, column = 1, columnspan = 5)

# Divers
labelCurSpeed = Label(frameMisc, text="Vitesse du curseur(%)") ; labelCurSpeed.grid(row = 1, column = 1)
scaleCurSpeed = Scale(frameMisc, orient=HORIZONTAL ,from_=50, to=200, variable = 100) ; scaleCurSpeed.grid(row = 1, column = 2, columnspan = 3)
scaleCurSpeed.set(100)

labelLvl = Label(frameMisc, text="Dossier des niveaux") ; labelLvl.grid(row = 2, column = 1)
entryLvl = Entry(frameMisc) ; entryLvl.grid(row = 2, column = 2)
buttonBrowse = Button(frameMisc) ; buttonBrowse.grid(row = 2, column = 3, padx = 5)

# Action
buttonSave = Button(frameAction, text = "Savegarder et quitter", command=save); buttonSave.grid(row = 1, column = 1, padx = 10)
buttonReset = Button(frameAction, text = "Utiliser valeurs par défaut et quitter", command=defaults); buttonReset.grid(row = 1, column = 2, padx = 10)
buttonCancel = Button(frameAction, text = "Quitter sans modifier", command=WINDOW.destroy); buttonCancel.grid(row = 1, column = 3, padx = 10)

print((entryResWidth.get(), entryResHeight.get()), entryIps.get(), fullscreen.get() ,scaleCurSpeed.get(), entryLvl.get())

WINDOW.mainloop()