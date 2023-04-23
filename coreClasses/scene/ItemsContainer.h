//
// Created by S.Morgenthaler on 23/04/2023.
//

#ifndef JUMPERBALLAPPLICATION_ITEMSCONTAINER_H
#define JUMPERBALLAPPLICATION_ITEMSCONTAINER_H

class ItemsContainer;

using CstItemsContainer_sptr = std::shared_ptr<ItemsContainer const>;

class ItemsContainer {

public:
    virtual unsigned int getCurrentNumberOfKeys() const = 0;

    virtual unsigned int getMaxNumberOfKeys() const = 0;

    virtual ~ItemsContainer() = default;
};


#endif //JUMPERBALLAPPLICATION_ITEMSCONTAINER_H
