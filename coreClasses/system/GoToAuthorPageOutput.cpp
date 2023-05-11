//
// Created by S.Morgenthaler on 11/05/2023.
//

#include "GoToAuthorPageOutput.h"

GoToAuthorPageOutput::GoToAuthorPageOutput(std::string &&author):
UpdateOutput("author", std::move(author)){

}
