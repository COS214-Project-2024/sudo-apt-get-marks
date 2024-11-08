#include "Utilities.h"
#include <iostream>

// Utility class implementation
Utility::Utility() : efficiency(100.0f)
{
    concrete = Concrete::getInstance();
    wood = Wood::getInstance();
    steel = Steel::getInstance();
    budget = Budget::getInstance();
    isFunctional = true;
}

Utility::~Utility() {}

// PowerPlant class implementation
PowerPlant::PowerPlant(powerPlants type, bool built, int powerGen) : powerGeneration(0), type(type)
{
    if (built)
    {
        srand(time(0));
        int randomNumber = (rand() % 5) + 1;
        powerGeneration = randomNumber * 1000;

        if (concrete->getKilo() < powerGeneration || steel->getKilo() < powerGeneration)
        {
            std::cout << "Could not create new PowerPlant" << std::endl;
            powerGeneration = 0;
            return;
        }

        concrete->setKilo(concrete->getKilo() - powerGeneration);
        steel->setKilo(steel->getKilo() - powerGeneration);

        std::cout << "Used " << powerGeneration << " kgs of concrete and steel to create a power plant" << std::endl;

        energy = Energy::getInstance();
        switch (this->type)
        {
        case HYDRO:
            energy->setWattsHydro(energy->getWattsHydro() + powerGeneration);
            break;
        case COAL:
            energy->setWattsCoal(energy->getWattsCoal() + powerGeneration);
            break;
        case WIND:
            energy->setWattsWind(energy->getWattsWind() + powerGeneration);
            break;
        case SOLAR:
            energy->setWattsSolar(energy->getWattsSolar() + powerGeneration);
            break;
        default:
            break;
        }
    }
    else
    {
        powerGeneration = powerGen;
        this->type = type;
        energy = Energy::getInstance();
    }
}

PowerPlant::~PowerPlant() {}

int PowerPlant::getPowerGeneration()
{
    return powerGeneration * (efficiency / 100.0f);
}

powerPlants PowerPlant::getType()
{
    return type;
}

void PowerPlant::generate()
{
    std::cout << "Generating power..." << std::endl;
}

// FunctionalPowerPlant class implementation
FunctionalPowerPlant::FunctionalPowerPlant(powerPlants type, bool built, int powerGen) : PowerPlant(type, built) {}

FunctionalPowerPlant::~FunctionalPowerPlant() {}

PowerPlant *FunctionalPowerPlant::repair()
{
    return this; // Already functional
}

PowerPlant *FunctionalPowerPlant::mulfunction()
{
    if (powerGeneration == 0)
    {
        std::cout << "Utility does not exist" << std::endl;
        return this;
    }
    isFunctional = false;
    NonFunctionalPowerPlant *nonFunctional = new NonFunctionalPowerPlant(this->getType(), false, powerGeneration);
    nonFunctional->setPowerGeneration(this->getPowerGeneration());
    nonFunctional->setEfficiency(this->getEfficiency() * 0.333);
    nonFunctional->setFunctional(false);

    switch (this->type)
    {
    case HYDRO:
        energy->setWattsHydro(energy->getWattsHydro() - powerGeneration * getEfficiency() + powerGeneration * nonFunctional->getEfficiency());
        break;
    case COAL:
        energy->setWattsCoal(energy->getWattsCoal() - powerGeneration * getEfficiency() + powerGeneration * nonFunctional->getEfficiency());
        break;
    case WIND:
        energy->setWattsWind(energy->getWattsWind() - powerGeneration * getEfficiency() + powerGeneration * nonFunctional->getEfficiency());
        break;
    case SOLAR:
        energy->setWattsSolar(energy->getWattsSolar() - powerGeneration * getEfficiency() + powerGeneration * nonFunctional->getEfficiency());
        break;
    default:
        break;
    }

    // std::cout << "Power plant malfunctioned." << std::endl;
    return nonFunctional;
}

// NonFunctionalPowerPlant class implementation
NonFunctionalPowerPlant::NonFunctionalPowerPlant(powerPlants type, bool built, int powerGen) : PowerPlant(type, built) {}

NonFunctionalPowerPlant::~NonFunctionalPowerPlant() {}

PowerPlant *NonFunctionalPowerPlant::repair()
{
    if (powerGeneration == 0)
    {
        std::cout << "Utility does not exist" << std::endl;
        return this;
    }

    if (concrete->getKilo() < powerGeneration * 0.2 && steel->getKilo() < powerGeneration * 0.2)
    {
        std::cout << "Power plant repair failed due to lack of materials" << std::endl;
        return this;
    }

    concrete->setKilo(concrete->getKilo() - powerGeneration * 0.2);
    steel->setKilo(steel->getKilo() - powerGeneration * 0.2);

    FunctionalPowerPlant *functional = new FunctionalPowerPlant(this->getType(), false, powerGeneration);
    functional->setPowerGeneration(this->getPowerGenerationRaw());
    functional->setEfficiency(100.0f); // Reset efficiency to 100%

    switch (this->type)
    {
    case HYDRO:
        energy->setWattsHydro(energy->getWattsHydro() - powerGeneration * getEfficiency() + powerGeneration * functional->getEfficiency());
        break;
    case COAL:
        energy->setWattsCoal(energy->getWattsCoal() - powerGeneration * getEfficiency() + powerGeneration * functional->getEfficiency());
        break;
    case WIND:
        energy->setWattsWind(energy->getWattsWind() - powerGeneration * getEfficiency() + powerGeneration * functional->getEfficiency());
        break;
    case SOLAR:
        energy->setWattsSolar(energy->getWattsSolar() - powerGeneration * getEfficiency() + powerGeneration * functional->getEfficiency());
        break;
    default:
        break;
    }

    std::cout << "Power plant repaired." << std::endl;
    functional->setFunctional(true);
    return functional;
}

PowerPlant *NonFunctionalPowerPlant::mulfunction()
{
    return this; // Already non-functional
}

// WaterSupply class implementation
WaterSupply::WaterSupply(bool built, int powerGen) : waterGeneration(0)
{
    if (built)
    {
        srand(time(0));
        int randomNumber = (rand() % 5) + 1;
        waterGeneration = randomNumber * 1000;

        if (concrete->getKilo() < waterGeneration || wood->getKilo() < waterGeneration)
        {
            std::cout << "Could not create new water supply" << std::endl;
            waterGeneration = 0;
            return;
        }

        concrete->setKilo(concrete->getKilo() - waterGeneration);
        wood->setKilo(wood->getKilo() - waterGeneration);

        std::cout << "Used " << waterGeneration << " kgs of concrete and wood to create a Water Supply" << std::endl;

        water = Water::getInstance();
        water->setliters(water->getLiters() + waterGeneration);
    }
    else
    {
        waterGeneration = powerGen;
        water = Water::getInstance();
    }
}

WaterSupply::~WaterSupply() {}

int WaterSupply::getWaterGeneration()
{
    return waterGeneration * (efficiency / 100.0f);
}

void WaterSupply::distribute()
{
    std::cout << "Distributing " << waterGeneration * efficiency / 100 << " liters of water..." << std::endl;
}

// FunctionalWaterSupply class implementation
FunctionalWaterSupply::FunctionalWaterSupply(bool built, int powerGen) : WaterSupply(built) {}

FunctionalWaterSupply::~FunctionalWaterSupply() {}

WaterSupply *FunctionalWaterSupply::repair()
{
    return this; // Already functional
}

WaterSupply *FunctionalWaterSupply::mulfunction()
{
    if (waterGeneration == 0)
    {
        std::cout << "Utility does not exist" << std::endl;
        return this;
    }
    NonFunctionalWaterSupply *nonFunctional = new NonFunctionalWaterSupply(false, waterGeneration);
    nonFunctional->setWaterGeneration(this->getWaterGenerationRaw());
    nonFunctional->setEfficiency(this->getEfficiency() * 0.333);
    water->setliters(water->getLiters() - waterGeneration * getEfficiency() + waterGeneration * nonFunctional->getEfficiency());
    // std::cout << "Water supply malfunctioned." << std::endl;
    nonFunctional->setFunctional(false);
    return nonFunctional;
}

// NonFunctionalWaterSupply class implementation
NonFunctionalWaterSupply::NonFunctionalWaterSupply(bool built, int powerGen) : WaterSupply(built) {}

NonFunctionalWaterSupply::~NonFunctionalWaterSupply() {}

WaterSupply *NonFunctionalWaterSupply::repair()
{
    if (waterGeneration == 0)
    {
        std::cout << "Utility does not exist" << std::endl;
        return this;
    }

    if (concrete->getKilo() < waterGeneration * 0.2 && wood->getKilo() < waterGeneration * 0.2)
    {
        std::cout << "Water repair failed due to lack of resources." << std::endl;
        return this;
    }

    concrete->setKilo(concrete->getKilo() - waterGeneration * 0.2);
    wood->setKilo(wood->getKilo() - waterGeneration * 0.2);

    FunctionalWaterSupply *functional = new FunctionalWaterSupply(false, waterGeneration);
    functional->setWaterGeneration(this->getWaterGenerationRaw());
    functional->setEfficiency(100.0f); // Reset efficiency to 100%

    water->setliters(water->getLiters() - waterGeneration * getEfficiency() + waterGeneration * functional->getEfficiency());
    std::cout << "Water supply repaired." << std::endl;
    functional->setFunctional(true);
    return functional;
}

WaterSupply *NonFunctionalWaterSupply::mulfunction()
{
    return this; // Already non-functional
}

// WasteManagement class implementation
WasteManagement::WasteManagement(bool built, int powerGen) : wasteCollection(0)
{
    if (built)
    {
        srand(time(0));
        int randomNumber = (rand() % 5) + 1;
        wasteCollection = randomNumber * 1000;

        if (steel->getKilo() < wasteCollection || wood->getKilo() < wasteCollection)
        {
            std::cout << "Waste management failed due to lack of resources." << std::endl;
            wasteCollection = 0;
            return;
        }

        steel->setKilo(steel->getKilo() - wasteCollection);
        wood->setKilo(wood->getKilo() - wasteCollection);

        std::cout << "Used " << wasteCollection << " kgs of steel and wood to create a Waste Management" << std::endl;
    }
    else
    {
        wasteCollection = powerGen;
    }
}

WasteManagement::~WasteManagement() {}

int WasteManagement::getWasteCollection()
{
    return wasteCollection * (efficiency / 100.0f);
}

void WasteManagement::recycle()
{
    std::cout << "Recycling " << wasteCollection * efficiency / 100 << " kgs of waste..." << std::endl;
}

void WasteManagement::collect()
{
    std::cout << "Collecting " << wasteCollection * efficiency / 100 << " kgs of waste..." << std::endl;
}

// FunctionalWasteManagement class implementation
FunctionalWasteManagement::FunctionalWasteManagement(bool built, int powerGen) : WasteManagement(built) {}

FunctionalWasteManagement::~FunctionalWasteManagement() {}

WasteManagement *FunctionalWasteManagement::repair()
{
    return this; // Already functional
}

WasteManagement *FunctionalWasteManagement::mulfunction()
{
    if (wasteCollection == 0)
    {
        std::cout << "Utility does not exist" << std::endl;
        return this;
    }
    NonFunctionalWasteManagement *nonFunctional = new NonFunctionalWasteManagement(false, wasteCollection);
    nonFunctional->setWasteCollection(this->getWasteCollectionRaw());
    nonFunctional->setEfficiency(this->getEfficiency() * 0.333);
    // std::cout << "Waste management malfunctioned." << std::endl;
    nonFunctional->setFunctional(false);
    return nonFunctional;
}

// NonFunctionalWasteManagement class implementation
NonFunctionalWasteManagement::NonFunctionalWasteManagement(bool built, int powerGen) : WasteManagement(built) {}

NonFunctionalWasteManagement::~NonFunctionalWasteManagement() {}

WasteManagement *NonFunctionalWasteManagement::repair()
{
    if (wasteCollection == 0)
    {
        std::cout << "Utility does not exist" << std::endl;
        return this;
    }
    if (steel->getKilo() < wasteCollection * 0.2 && wood->getKilo() < wasteCollection * 0.2)
    {
        std::cout << "Waste management repair failed due to lack of resources." << std::endl;
        wasteCollection = 0;
        return this;
    }

    steel->setKilo(steel->getKilo() - wasteCollection * 0.2);
    wood->setKilo(wood->getKilo() - wasteCollection * 0.2);

    FunctionalWasteManagement *functional = new FunctionalWasteManagement(false, wasteCollection);
    functional->setWasteCollection(this->getWasteCollectionRaw());
    functional->setEfficiency(100.0f); // Reset efficiency to 100%
    std::cout << "Waste management repaired." << std::endl;
    functional->setFunctional(true);
    return functional;
}

WasteManagement *NonFunctionalWasteManagement::mulfunction()
{
    return this; // Already non-functional
}