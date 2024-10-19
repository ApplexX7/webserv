/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeType.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:22:10 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/16 15:41:06 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"


std::string Response::getMimeType(std::string &Mimetype){
    if (Mimetype == "text/pain"){
        return (".txt");
    }
    else if (Mimetype == "text/html"){
        return (".html");
    }
    else if (Mimetype == "text/css"){
        return (".css");
    }
    else if (Mimetype == "text/javascript"){
        return (".js");
    }
    else if (Mimetype == "text/markdown"){
        return (".js");
    }
    else if (Mimetype == "text/csv"){
        return (".csv");
    }
    else if (Mimetype == "text/xml"){
        return (".xml");
    }
    else if (Mimetype == "image/jpeg"){
        return (".jpg");
    }
    else if (Mimetype == "image/png"){
        return (".png");
    }
    else if (Mimetype == "image/gif"){
        return (".gif");
    }
    else if (Mimetype == "image/bmp"){
        return (".bmp");
    }
    else if (Mimetype == "image/svg+xml"){
        return (".svg");
    }
    else if (Mimetype == "image/tiff"){
        return (".tiff");
    }
    else if (Mimetype ==  "image/webp"){
        return (".webp");
    }
    else if (Mimetype == "audio/mpeg"){
        return (".mp3");
    }
    else if (Mimetype == "audio/ogg"){
        return (".ogg");
    }
    else if (Mimetype == "audio/wav"){
        return (".wav");
    }
    else if (Mimetype == "audio/webm"){
        return (".webm");
    }
    else if (Mimetype ==  "audio/flac"){
        return (".flac");
    }
    else if (Mimetype == "audio/aac"){
        return (".aac");
    }
    else if (Mimetype == "video/mp4"){
        return (".mp4");
    }
    else if (Mimetype == "video/ogg"){
        return (".ogv");
    }
    else if (Mimetype == "video/webm"){
        return (".webm");
    }
    else if (Mimetype == "video/x-msvideo"){
        return (".avi");
    }
    else if (Mimetype == "video/quicktime"){
        return (".mov");
    }
    else if (Mimetype == "video/mpeg"){
        return (".mpeg");
    }
    else if (Mimetype == "video/x-flv"){
        return (".flv");
    }
    else if (Mimetype == "application/json"){
        return (".json");
    }
    else if (Mimetype == "application/xml"){
        return (".xml");
    }
    else if (Mimetype == "application/pdf"){
        return (".pdf");
    }
    else if (Mimetype == "application/zip"){
        return (".zip");
    }
    else if (Mimetype == "application/gzip"){
        return (".gz");
    }
    else if (Mimetype == "application/x-tar"){
        return (".tar");
    }
    else if (Mimetype == "application/x-7z-compressed"){
        return (".7z");
    }
    else if (Mimetype == "application/vnd.ms-excel"){
        return (".xls");
    }
    else if (Mimetype == "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"){
        return (".xlsx");
    }
    else if (Mimetype == "application/vnd.ms-powerpoint"){
        return (".ppt");
    }
    else if (Mimetype == "application/vnd.openxmlformats-officedocument.presentationml.presentation"){
        return (".pptx");
    }
    else if (Mimetype == "application/msword"){
        return (".doc");
    }
    else if (Mimetype == "application/vnd.openxmlformats-officedocument.wordprocessingml.document"){
        return (".docx");
    }
    else if (Mimetype == "application/x-www-form-urlencoded"){
        return ("");
    }
    else if (Mimetype == "application/octet-stream")
        return ("");
    else  if (Mimetype == "application/ld+json"){
        return (".jsonld");
    }
    else if (Mimetype == "multipart/form-data"){
        return ("");
    }
    else
        return ("");
}