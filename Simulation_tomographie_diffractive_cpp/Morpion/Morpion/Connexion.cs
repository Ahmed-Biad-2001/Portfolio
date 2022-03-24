using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Drawing;
using System.Globalization;

namespace Morpion
{
    // On définit les différents type de trame, qui seront utilisé par EcrireTrame et LireTrame
    public enum TrameType : Byte { Apparence, CaseJouee, EtatDuJeu, Gagnant, Ping, Message };
    // Les trames sont composées d'un premier byte qui désigne le type, puis des arguments détaillés ci-dessous
    
    // Apparence: couleur en hexa sur 6 bytes , pseudo sur 4 bytes
    // CaseJoue: la case sur 1 byte (de 0 à 8)
    // EtatDuJeu: grille: 9 bytes (S,C,-), tour: 1 byte (S,C,-), scoreS: 1 byte (1 nombre), scoreC: 1 byte (1 nombre)
    
    // Le reste des trames n'ont pas été utilisés dans ce programme

    public abstract class Connexion
    {
        public bool connecte = false;
        public bool pseudoInitialise = false; // Devient true lorsqu'on reçoit le pseudo et couleur de l'adversaire
        public EtatFenetre etatFenetre = new EtatFenetre(); 

        public Color couleurAdverse;
        public string pseudoAdverse;

        protected NetworkStream networkStream; // La connexion du serveur et client utilisent tous les deux networkstream pour le flux de données

        // Cette fonction est appelé pérodiquement pour vérifier si l'autre machine a envoyé une trame
        public void LireTrame()
        {
            Byte[] msg = new byte[256];
            try {
                int size = networkStream.Read(msg, 0, msg.Length); // On obtient la taille de la trame dans size, et son contenu dans msg

                string msg2 = System.Text.Encoding.ASCII.GetString(msg, 0, size); 
                Console.WriteLine("Trame reçue: *{0}", msg2);

                if (size != 0) TraiterTrame(msg); // TraiterTrame est défini différement pour le serveur et client
            }
            catch { } // Ignorer l'exception si rien n'est reçu
        }

        protected virtual void TraiterTrame(Byte[] trame) { }

        // Envoie une trame à l'adversaire. On passe à cette fonction le type de trame et les arguments sous forme de tableau de string
        protected void EcrireTrame(TrameType pType, string[] args)
        {
            string msg = (char)pType + "";
            for (int i = 0; i < args.Length; i++)
            {
                msg = msg + " " + args[i];
            }
            networkStream.Write( Encoding.ASCII.GetBytes(msg) , 0, msg.Length);
            Console.WriteLine("Trame envoyee: *{0}", msg);
        }

        // On envoie la trame initiale à l'autre machine
        public void EcrireTrameApparence(string pseudo, Color couleur)
        {
            // Exemple: couleur = Color.Blue;

            int intCouleur = ColorTranslator.ToOle(couleur); // intCouleur = 0x00FF00
            string strCouleur = intCouleur.ToString("X"); // strCouleur = "FF00"
            strCouleur = new string('0', 6 - strCouleur.Length) + strCouleur;  // strCouleur = "00FF00"


            int pseudoLen = pseudo.Length < 4 ? pseudo.Length : 4;
            string strPseudo = pseudo.Substring(0, pseudoLen); // On vérifie que ceci ne dépasse pas 4 caractères

            EcrireTrame(TrameType.Apparence, new string[] { strCouleur, strPseudo } );
        }

        // On reçoit la trame initiale de l'autre machine
        protected void TraiterTrameApparence(Byte[] trame)
        {  
            Byte[] byteColor = trame.Skip(2).Take(6).ToArray(); // byteColor = (byte)"00FF00"
            string strColor = Encoding.ASCII.GetString(byteColor); // byteColor = "00FF00"
            int intColor = Int32.Parse(strColor, NumberStyles.HexNumber); // intColor = 0x00FF00
            Console.WriteLine(strColor);
            couleurAdverse = ColorTranslator.FromOle(intColor);

            Byte[] bytePseudo = trame.Skip(9).Take(4).ToArray();
            pseudoAdverse = Encoding.ASCII.GetString(bytePseudo);

            pseudoInitialise = true;
        }

        public virtual void ClickCase(int c) { }

        public abstract void Update();

        public virtual void Quitter()
        {
            
        }
    }
}
