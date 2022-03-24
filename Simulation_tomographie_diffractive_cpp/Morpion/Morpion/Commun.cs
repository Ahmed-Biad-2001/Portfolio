using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Drawing;

namespace Morpion
{
    // Enumeration designant le joueur
    public enum IdJoueur: Byte { Personne = (byte)'-', Serveur = (byte)'S', Client = (byte)'C' }

    // Structure pour les parametres passés de la 1ere fenetre à la 2e
    public class Parametres
    {
        public bool jeSuisServeur;                  // vaut vrai lorsque la machine représente le serveur, faux sinon
        public string adrIP_serveur = "localhost";
        public Color colorServeur = Color.White;    // couleur du serveur
        public Color colorClient = Color.White;     // couleur du client
        public string charServeur = "X";            // pseudo du serveur
        public string charClient = "O";             // pseudo du client
    }
    
    // Structure que lit la 2e fenetre pour mettre à jour son contenu
    public class EtatFenetre
    {
        public IdJoueur[] grille = new IdJoueur[9];
        public IdJoueur tour;
        public int scoreS, scoreC;
    }
}
