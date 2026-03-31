// ========================================
// CONFIGURATION FIREBASE
// Smart Study Desk Monitor
// ========================================

const firebaseConfig = {
    apiKey: "",
    authDomain: "",
    databaseURL: "",
    projectId: "",
    storageBucket: "",
    messagingSenderId: "",
    appId: ""
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
