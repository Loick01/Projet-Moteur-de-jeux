#include <ParamsWindow.hpp>

char ParamsWindow::nameStructure[512]; // Permet d'éviter les erreurs de lien à la compilation

ParamsWindow::ParamsWindow(GLFWwindow* window, int style, TerrainControler *terrainControler, Player *player){
    this->style = style;
    this->renduFilaire = false;
    this->terrainControler = terrainControler;
    this->mg = terrainControler->getMapGenerator();
    this->inEditor = false;
    this->clearEntity = false;
    this->speedPlayer = player->getRefToSpeed();
    this->posJoueur = player->getHitbox()->getRefToBottomPoint();
    this->planeWidth = terrainControler->getRefToPlaneWidth();
    this->planeLength = terrainControler->getRefToPlaneLength();
    this->seedTerrain = terrainControler->getRefToSeedTerrain();
    this->octave = terrainControler->getRefToOctave();
    this->hitboxPlayer = player->getHitbox();
    this->init(window);
    this->useStyle();
}

ParamsWindow::~ParamsWindow(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // Les champs qui sont dans cette classe sont en grande partie des pointeurs vers des champs
    // d'autres classes, ils seront supprimés dans leurs classes respectives
    // Il n'y a donc rien d'autre à delete ici
}

void ParamsWindow::useStyle(){
    if( this->style == 0){
        ImGui::StyleColorsDark();
    }else if (this->style == 1){
        ImGui::StyleColorsLight();
    }else{
        ImGui::StyleColorsClassic();
    }
}

void ParamsWindow::init(GLFWwindow* window){
    // Setup ImGui binding
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

bool ParamsWindow::getInEditor(){
    return this->inEditor;
}

bool ParamsWindow::getClearEntity(){
	return this->clearEntity;
}

void ParamsWindow::resetClearEntity(){
	this->clearEntity = false;
}

void ParamsWindow::modifTerrain(){
	this->clearEntity = true; // On fera disparaître les entités au moment où on change le terrain
    this->mg->generateImage();
    int widthHeightmap, heightHeightmap, channels;
    unsigned char* dataPixels = stbi_load("../Textures/terrain.png", &widthHeightmap, &heightHeightmap, &channels, 4);
    this->terrainControler->buildPlanChunks(dataPixels, widthHeightmap, heightHeightmap);
    stbi_image_free(dataPixels);
    this->hitboxPlayer->resetCanTakeDamage(); // Le joueur ne prends pas de dégâts de chute s'il tombe de trop haut au moment du changement de terrain
}

void ParamsWindow::draw(){
    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    float fps = ImGui::GetIO().Framerate;

    ImGui::NewFrame();
    ImGui::Begin("Panneau de contrôle");
    if (ImGui::SliderInt("Style de la fenêtre ImGui", &(this->style), 0, 2)){
        this->useStyle();
    }
    ImGui::Text("FPS : %.2f", fps);

    ImGui::Spacing();

    ImGui::SliderInt("Vitesse caméra", &speedCam, 5, 200);

    ImGui::Spacing();

    if(!this->inEditor){

        ImGui::Text("Position : %.2f / %.2f / %.2f", (*posJoueur)[0], (*posJoueur)[1], (*posJoueur)[2]);

        ImGui::Spacing();

        ImGui::SliderFloat("Vitesse Joueur", this->speedPlayer, 0.0, 50.0);

        ImGui::Spacing();

        ImGui::SliderFloat("FoV", &FoV, 1.0, 179.9);

        ImGui::Spacing();
    }

    if (ImGui::Checkbox("Caméra orbitale", &cameraOrbitale)){
        cameraLibre = false;
        cameraMouseLibre = false;
        cameraMousePlayer = false;
    }

    ImGui::Spacing();
    
    if (ImGui::Checkbox("Caméra libre", &cameraLibre)){
        cameraOrbitale = false;
        cameraMouseLibre = false;
        cameraMousePlayer = false;
    }

    ImGui::Spacing();
    
    if (ImGui::Checkbox("Caméra souris", &cameraMouseLibre)){
        cameraLibre = false;
        cameraOrbitale = false;
        cameraMousePlayer = false;
    }

    if (!this->inEditor){
        if (ImGui::Checkbox("Caméra player", &cameraMousePlayer)){
            cameraLibre = false;
            cameraOrbitale = false;
            cameraMouseLibre = false;
        }
    }

    ImGui::Spacing();

    if (ImGui::Checkbox("Rendu filaire", &renduFilaire)) {
        if (renduFilaire) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
    ImGui::Spacing();

    ImGui::Checkbox("Afficher l'hud", &showHud);

    ImGui::Spacing();

    ImGui::Checkbox("Mode de jeu (créatif/survie)", &modeJeu);

    ImGui::Spacing();

    if(!(this->inEditor)){
        ImGui::Checkbox("Mode éditeur", &(this->inEditor));
    }

    ImGui::Spacing();

    if(this->inEditor){
        ImGui::Text("Nom du fichier de la structure : ");
        ImGui::SameLine();
        ImGui::InputText(" ", nameStructure, IM_ARRAYSIZE(nameStructure));
        if (ImGui::Button("Sauvegarder la structure")){
            std::string filePath = nameStructure;
            if (filePath.size() != 0){
                this->terrainControler->saveStructure(filePath);
            }else{
                std::cout << "Veuillez saisir un nom pour le fichier de la structure\n";
            }
        }
    }else{
        if (ImGui::SliderInt("Longueur terrain", this->planeWidth, 1, 22)){
            this->mg->setWidthMap(*(this->planeWidth));
            this->modifTerrain();
        }

        ImGui::Spacing();

        if (ImGui::SliderInt("Largeur terrain", this->planeLength, 1, 22)){
            this->mg->setHeightMap(*(this->planeLength));
            this->modifTerrain();
        }

        ImGui::Spacing();

        if(ImGui::SliderInt("Seed de génération", this->seedTerrain, 0, 10000)){
            this->mg->setSeed(*(this->seedTerrain));
        }

        ImGui::Spacing();

        if(ImGui::SliderInt("Nombre d'octaves", this->octave, 1, 7)){
            this->mg->setOctave(*(this->octave));
        }

        ImGui::Spacing();

        if (ImGui::Button("Utiliser la seed et le nombre d'octaves")){
            this->modifTerrain();
        }

        ImGui::Spacing();

        /*
        // Type 0 = Plein ; Type 1 = Sinus ; Type 2 = Flat ; Type 3 = Procedural
        if (ImGui::SliderInt("Type de chunk", &typeChunk, 0, 3)){
            buildPlanChunks(dataPixels, widthHeightmap, heightHeightmap);
        }
        */
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ParamsWindow::attachNewTerrain(TerrainControler *terrainControler){
    this->terrainControler = terrainControler;
    // Attention à bien récupérer les références de la nouvelle instance de TerrainControler
    this->mg = terrainControler->getMapGenerator();
    this->planeWidth = terrainControler->getRefToPlaneWidth();
    this->planeLength = terrainControler->getRefToPlaneLength();
    this->seedTerrain = terrainControler->getRefToSeedTerrain();
    this->octave = terrainControler->getRefToOctave();
}
