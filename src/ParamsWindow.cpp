#include <ParamsWindow.hpp>

char ParamsWindow::nameStructure[512]; // Permet d'éviter les erreurs de lien à la compilation

ParamsWindow::ParamsWindow(GLFWwindow* window, int style, TerrainControler *terrainControler){
    this->style = style;
    this->renduFilaire = false;
    this->terrainControler = terrainControler;
    this->inEditor = false;
    this->init(window);
    this->useStyle();
}

ParamsWindow::~ParamsWindow(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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

    ImGui::SliderInt("Vitesse caméra", &speedCam, 5, 100);

    ImGui::Spacing();

    if(!this->inEditor){

        //ImGui::Text("Position : %.2f / %.2f / %.2f", bottomPointPlayer[0], bottomPointPlayer[1], bottomPointPlayer[2]);

        ImGui::Spacing();

        //ImGui::SliderFloat("Vitesse Joueur", &playerSpeed, 0.0, 50.0);

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

    ImGui::Checkbox("Mode éditeur", &(this->inEditor));

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
        /*
        if (ImGui::SliderInt("Longueur terrain", &planeWidth, 1, 32)){
            mg->setWidthMap(planeWidth);
            mg->generateImage();
            dataPixels = stbi_load("../Textures/terrain.png", &widthHeightmap, &heightHeightmap, &channels, 4);
            buildPlanChunks(dataPixels, widthHeightmap, heightHeightmap);
        }
        */

        ImGui::Spacing();

        /*
        if (ImGui::SliderInt("Largeur", &planeLength, 1, 32)){
            mg->setHeightMap(planeLength);
            mg->generateImage();
            dataPixels = stbi_load("../Textures/terrain.png", &widthHeightmap, &heightHeightmap, &channels, 4);
            buildPlanChunks(dataPixels, widthHeightmap, heightHeightmap);
        }
        */

        ImGui::Spacing();

        /*
        if(ImGui::SliderInt("Seed de génération", &seedTerrain, 0, 10000)){
            mg->setSeed(seedTerrain);
        }
        */

        ImGui::Spacing();

        /*
        if(ImGui::SliderInt("Nombre d'octaves", &octave, 1, 10)){
            mg->setOctave(octave);
        }
        */

        ImGui::Spacing();

        /*
        if (ImGui::Button("Utiliser la seed et le nombre d'octaves")){
            mg->generateImage();
            dataPixels = stbi_load("../Textures/terrain.png", &widthHeightmap, &heightHeightmap, &channels, 4);
            buildPlanChunks(dataPixels, widthHeightmap, heightHeightmap);
        }
        */

        // Pour simplifier, on limite à un seul chunk de haut
        //if (ImGui::SliderInt("Hauteur", &planeHeight, 1, 8)){
        //    buildPlanChunks(dataPixels, widthHeightmap, heightHeightmap);
        //}

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
}