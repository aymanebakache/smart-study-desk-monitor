#!/bin/bash

# ========================================
# Script d'initialisation du projet
# Smart Study Desk Monitor
# ========================================

echo "╔═══════════════════════════════════════════════════════════╗"
echo "║  Smart Study Desk Monitor - Initialisation du Projet     ║"
echo "╚═══════════════════════════════════════════════════════════╝"
echo ""

# Couleurs pour output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# ========================================
# Vérification des prérequis
# ========================================

echo -e "${BLUE}📋 Vérification des prérequis...${NC}"
echo ""

# Vérifier Node.js
if command -v node &> /dev/null; then
    NODE_VERSION=$(node -v)
    echo -e "${GREEN}✓ Node.js installé: ${NODE_VERSION}${NC}"
else
    echo -e "${RED}✗ Node.js n'est pas installé${NC}"
    echo "  Installez Node.js depuis https://nodejs.org/"
    exit 1
fi

# Vérifier npm
if command -v npm &> /dev/null; then
    NPM_VERSION=$(npm -v)
    echo -e "${GREEN}✓ npm installé: ${NPM_VERSION}${NC}"
else
    echo -e "${RED}✗ npm n'est pas installé${NC}"
    exit 1
fi

# Vérifier Git
if command -v git &> /dev/null; then
    GIT_VERSION=$(git --version)
    echo -e "${GREEN}✓ Git installé: ${GIT_VERSION}${NC}"
else
    echo -e "${YELLOW}⚠ Git n'est pas installé (recommandé)${NC}"
fi

# Vérifier Python (optionnel pour PlatformIO)
if command -v python3 &> /dev/null; then
    PYTHON_VERSION=$(python3 --version)
    echo -e "${GREEN}✓ Python installé: ${PYTHON_VERSION}${NC}"
else
    echo -e "${YELLOW}⚠ Python n'est pas installé (nécessaire pour PlatformIO)${NC}"
fi

# Vérifier PlatformIO (optionnel)
if command -v pio &> /dev/null; then
    PIO_VERSION=$(pio --version)
    echo -e "${GREEN}✓ PlatformIO installé: ${PIO_VERSION}${NC}"
else
    echo -e "${YELLOW}⚠ PlatformIO n'est pas installé (nécessaire pour ESP32)${NC}"
    echo "  Installez-le via VS Code ou: pip install platformio"
fi

echo ""
echo -e "${BLUE}════════════════════════════════════════════════════════════${NC}"
echo ""

# ========================================
# Création de la structure de dossiers
# ========================================

echo -e "${BLUE}📁 Création de la structure de dossiers...${NC}"
echo ""

# Créer la structure
mkdir -p docs
mkdir -p hardware/src
mkdir -p hardware/lib
mkdir -p hardware/config
mkdir -p cloud/firebase
mkdir -p cloud/functions
mkdir -p web/public
mkdir -p web/src/components
mkdir -p web/src/services
mkdir -p web/src/utils
mkdir -p web/src/styles
mkdir -p tests/hardware
mkdir -p tests/web
mkdir -p assets/images
mkdir -p assets/diagrams
mkdir -p assets/presentations

echo -e "${GREEN}✓ Structure de dossiers créée${NC}"

# ========================================
# Création des fichiers .gitkeep
# ========================================

echo -e "${BLUE}📄 Création des fichiers .gitkeep...${NC}"

# Créer .gitkeep dans les dossiers vides
touch hardware/lib/.gitkeep
touch cloud/functions/.gitkeep
touch tests/hardware/.gitkeep
touch tests/web/.gitkeep
touch assets/images/.gitkeep
touch assets/diagrams/.gitkeep

echo -e "${GREEN}✓ Fichiers .gitkeep créés${NC}"

# ========================================
# Initialisation Git
# ========================================

echo ""
echo -e "${BLUE}🔧 Initialisation Git...${NC}"

if [ -d ".git" ]; then
    echo -e "${YELLOW}⚠ Repository Git déjà initialisé${NC}"
else
    git init
    echo -e "${GREEN}✓ Repository Git initialisé${NC}"
fi

# ========================================
# Installation des dépendances web
# ========================================

echo ""
echo -e "${BLUE}📦 Installation des dépendances web...${NC}"
echo ""

cd web

if [ -f "package.json" ]; then
    echo "Installation en cours..."
    npm install
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Dépendances web installées${NC}"
    else
        echo -e "${RED}✗ Erreur lors de l'installation des dépendances${NC}"
    fi
else
    echo -e "${YELLOW}⚠ package.json non trouvé dans web/${NC}"
fi

cd ..

# ========================================
# Installation Firebase CLI
# ========================================

echo ""
echo -e "${BLUE}🔥 Installation Firebase CLI...${NC}"

if command -v firebase &> /dev/null; then
    FIREBASE_VERSION=$(firebase --version)
    echo -e "${GREEN}✓ Firebase CLI déjà installé: ${FIREBASE_VERSION}${NC}"
else
    echo "Installation de Firebase CLI..."
    npm install -g firebase-tools
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Firebase CLI installé${NC}"
    else
        echo -e "${RED}✗ Erreur lors de l'installation de Firebase CLI${NC}"
    fi
fi

# ========================================
# Initialisation PlatformIO (si disponible)
# ========================================

echo ""
echo -e "${BLUE}⚡ Configuration PlatformIO...${NC}"

if command -v pio &> /dev/null; then
    cd hardware
    
    if [ -f "platformio.ini" ]; then
        echo -e "${GREEN}✓ platformio.ini déjà configuré${NC}"
    else
        echo "Initialisation de PlatformIO..."
        pio project init --board esp32dev
        echo -e "${GREEN}✓ Projet PlatformIO initialisé${NC}"
    fi
    
    cd ..
else
    echo -e "${YELLOW}⚠ PlatformIO non disponible - ignoré${NC}"
fi

# ========================================
# Création des fichiers README
# ========================================

echo ""
echo -e "${BLUE}📝 Création des fichiers README...${NC}"

# README principal (s'il n'existe pas)
if [ ! -f "README.md" ]; then
    cat > README.md << 'EOF'
# Smart Study Desk Monitor

Système IoT de surveillance des conditions d'étude avec tableau de bord cloud.

## 📁 Structure du Projet

- `hardware/` - Code ESP32 et firmware
- `cloud/` - Configuration Firebase
- `web/` - Dashboard web
- `docs/` - Documentation
- `tests/` - Tests unitaires

## 🚀 Démarrage rapide

Consultez le `GUIDE_VS_CODE_ARCHITECTURE.md` pour les instructions complètes.

## 🛠️ Technologies

- **Hardware**: ESP32, BH1750, DHT22, KY-038
- **Cloud**: Firebase Realtime Database
- **Web**: HTML5, CSS3, JavaScript, Chart.js

## 📚 Documentation

Voir le dossier `docs/` pour la documentation complète.
EOF
    echo -e "${GREEN}✓ README.md créé${NC}"
fi

# README hardware
if [ ! -f "hardware/README.md" ]; then
    cat > hardware/README.md << 'EOF'
# Hardware - ESP32 Code

Code firmware pour ESP32 avec capteurs.

## Compilation

```bash
pio run
```

## Téléversement

```bash
pio run --target upload
```

## Moniteur série

```bash
pio device monitor
```
EOF
    echo -e "${GREEN}✓ hardware/README.md créé${NC}"
fi

# README web
if [ ! -f "web/README.md" ]; then
    cat > web/README.md << 'EOF'
# Web Dashboard

Interface web pour visualisation des données.

## Installation

```bash
npm install
```

## Démarrage

```bash
npm start
```

## Build production

```bash
npm run build
```
EOF
    echo -e "${GREEN}✓ web/README.md créé${NC}"
fi

# ========================================
# Résumé final
# ========================================

echo ""
echo -e "${BLUE}════════════════════════════════════════════════════════════${NC}"
echo ""
echo -e "${GREEN}✅ Initialisation terminée avec succès!${NC}"
echo ""
echo -e "${YELLOW}📋 Prochaines étapes:${NC}"
echo ""
echo "1. Ouvrir le workspace dans VS Code:"
echo "   code smart-study-desk-monitor.code-workspace"
echo ""
echo "2. Configurer Firebase:"
echo "   cd cloud && firebase login && firebase init"
echo ""
echo "3. Configurer ESP32:"
echo "   Modifiez hardware/config/config.h avec vos identifiants"
echo ""
echo "4. Installer les extensions VS Code recommandées"
echo ""
echo -e "${BLUE}════════════════════════════════════════════════════════════${NC}"
echo ""
echo -e "${GREEN}Bon développement! 🚀${NC}"
echo ""
