// ========================================
// CONFIGURATION FIREBASE
// Smart Study Desk Monitor
// ========================================

/**
 * Pour obtenir votre configuration Firebase :
 * 
 * 1. Allez sur https://console.firebase.google.com/
 * 2. Sélectionnez votre projet
 * 3. Cliquez sur l'icône "Paramètres" (engrenage) > "Paramètres du projet"
 * 4. Dans l'onglet "Général", faites défiler jusqu'à "Vos applications"
 * 5. Si vous n'avez pas encore ajouté d'application Web :
 *    - Cliquez sur l'icône Web </> pour ajouter une app web
 *    - Donnez un nom à votre app (ex: "Dashboard")
 *    - Cochez "Configurer aussi Firebase Hosting" si vous voulez héberger
 * 6. Copiez l'objet firebaseConfig qui apparaît
 * 7. Collez-le ci-dessous en remplacement de l'exemple
 */

// ⚠️ REMPLACEZ CES VALEURS PAR VOS PROPRES INFORMATIONS FIREBASE
const firebaseConfig = {
    apiKey: "AIzaSyXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    authDomain: "smart-study-desk-monitor.firebaseapp.com",
    databaseURL: "https://smart-study-desk-monitor-default-rtdb.europe-west1.firebasedatabase.app",
    projectId: "smart-study-desk-monitor",
    storageBucket: "smart-study-desk-monitor.appspot.com",
    messagingSenderId: "123456789012",
    appId: "1:123456789012:web:abcdef123456789abcdef"
};

// Initialiser Firebase
firebase.initializeApp(firebaseConfig);

// Obtenir une référence à la base de données
const database = firebase.database();

// Exporter pour utilisation dans d'autres fichiers si nécessaire
if (typeof module !== 'undefined' && module.exports) {
    module.exports = { firebaseConfig, database };
}

console.log('✓ Firebase configuré et initialisé');
console.log('URL Database:', firebaseConfig.databaseURL);
