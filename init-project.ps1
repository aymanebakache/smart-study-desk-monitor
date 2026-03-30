# ========================================
# Script d'initialisation du projet (Windows)
# Smart Study Desk Monitor
# ========================================

Write-Host ""
Write-Host "===============================================" -ForegroundColor Cyan
Write-Host " Smart Study Desk Monitor - Initialisation "
Write-Host "===============================================" -ForegroundColor Cyan
Write-Host ""

# ========================================
# Vérification des prérequis
# ========================================

Write-Host "Verification des prerequis..." -ForegroundColor Blue
Write-Host ""

function Test-Command {
    param ($command)

    try {
        if (Get-Command $command -ErrorAction Stop) {
            return $true
        }
    } catch {
        return $false
    }
}

# Node.js
if (Test-Command "node") {
    Write-Host "Node.js detecte" -ForegroundColor Green
} else {
    Write-Host "Node.js non installe. https://nodejs.org" -ForegroundColor Red
}

# npm
if (Test-Command "npm") {
    Write-Host "npm detecte" -ForegroundColor Green
} else {
    Write-Host "npm non installe" -ForegroundColor Red
}

# Git
if (Test-Command "git") {
    Write-Host "Git detecte" -ForegroundColor Green
} else {
    Write-Host "Git non installe (optionnel)" -ForegroundColor Yellow
}

# Python
if (Test-Command "python") {
    Write-Host "Python detecte" -ForegroundColor Green
} else {
    Write-Host "Python non installe (necessaire pour PlatformIO)" -ForegroundColor Yellow
}

# PlatformIO
if (Test-Command "pio") {
    Write-Host "PlatformIO detecte" -ForegroundColor Green
} else {
    Write-Host "PlatformIO non installe (ESP32)" -ForegroundColor Yellow
}

Write-Host ""

# ========================================
# Création des dossiers
# ========================================

Write-Host "Creation de la structure du projet..." -ForegroundColor Blue

$folders = @(
    "docs",
    "hardware/src",
    "hardware/lib",
    "hardware/config",
    "cloud/firebase",
    "cloud/functions",
    "web/public",
    "web/src/components",
    "web/src/services",
    "web/src/utils",
    "web/src/styles",
    "tests/hardware",
    "tests/web",
    "assets/images",
    "assets/diagrams",
    "assets/presentations"
)

foreach ($folder in $folders) {
    if (!(Test-Path $folder)) {
        New-Item -ItemType Directory -Path $folder | Out-Null
    }
}

Write-Host "Structure creee" -ForegroundColor Green

# ========================================
# Fichiers .gitkeep
# ========================================

Write-Host "Creation des fichiers .gitkeep..."

$gitkeepFiles = @(
    "hardware/lib/.gitkeep",
    "cloud/functions/.gitkeep",
    "tests/hardware/.gitkeep",
    "tests/web/.gitkeep"
)

foreach ($file in $gitkeepFiles) {
    if (!(Test-Path $file)) {
        New-Item -ItemType File -Path $file | Out-Null
    }
}

Write-Host "gitkeep crees" -ForegroundColor Green

# ========================================
# Initialisation Git
# ========================================

Write-Host ""
Write-Host "Initialisation Git..."

if (!(Test-Path ".git")) {
    git init
    Write-Host "Repository Git initialise" -ForegroundColor Green
}
else {
    Write-Host "Repository Git deja existant" -ForegroundColor Yellow
}

# ========================================
# Installation dépendances Web
# ========================================

Write-Host ""
Write-Host "Installation dependances web..."

if (Test-Path "web/package.json") {

    Push-Location web
    npm install
    Pop-Location

    Write-Host "Dependances web installees" -ForegroundColor Green
}
else {
    Write-Host "package.json non trouve dans web/" -ForegroundColor Yellow
}

# ========================================
# Installation Firebase CLI
# ========================================

Write-Host ""
Write-Host "Verification Firebase CLI..."

if (Test-Command "firebase") {

    Write-Host "Firebase CLI deja installe" -ForegroundColor Green
}
else {

    Write-Host "Installation Firebase CLI..."

    npm install -g firebase-tools

    Write-Host "Firebase CLI installe" -ForegroundColor Green
}

# ========================================
# Configuration PlatformIO
# ========================================

Write-Host ""
Write-Host "Configuration PlatformIO..."

if (Test-Command "pio") {

    if (!(Test-Path "hardware/platformio.ini")) {

        Push-Location hardware
        pio project init --board esp32dev
        Pop-Location

        Write-Host "Projet ESP32 initialise" -ForegroundColor Green
    }
    else {

        Write-Host "platformio.ini deja present" -ForegroundColor Yellow
    }
}
else {

    Write-Host "PlatformIO non disponible - ignore" -ForegroundColor Yellow
}

# ========================================
# README principal
# ========================================

Write-Host ""
Write-Host "Creation README..."

if (!(Test-Path "README.md")) {

$readme = @"
# Smart Study Desk Monitor

Systeme IoT de surveillance des conditions d'etude.

## Structure

hardware  -> code ESP32
cloud     -> configuration Firebase
web       -> dashboard web
docs      -> documentation
tests     -> tests

## Technologies

ESP32
Firebase
JavaScript
Chart.js
"@

$readme | Set-Content README.md

Write-Host "README cree"
}

# ========================================
# Fin
# ========================================

Write-Host ""
Write-Host "==============================================="
Write-Host "Initialisation terminee"
Write-Host "==============================================="
Write-Host ""

Write-Host "Prochaines etapes:" -ForegroundColor Yellow
Write-Host ""
Write-Host "1. Ouvrir le projet dans VS Code"
Write-Host "code smart-study-desk-monitor.code-workspace"
Write-Host ""
Write-Host "2. Configurer Firebase"
Write-Host "firebase login"
Write-Host "firebase init"
Write-Host ""
Write-Host "3. Configurer ESP32 dans hardware/config"
Write-Host ""
Write-Host "Bon developpement !"
Write-Host ""