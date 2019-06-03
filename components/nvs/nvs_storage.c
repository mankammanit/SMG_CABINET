#include "nvs_storage.h"


void save_ratio(ratio_val ptr)
{
        // Open
        printf("\n");
        printf("Opening Non-Volatile Storage (NVS) handle...\n");
        nvs_handle my_handle;
        esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
        if (err != ESP_OK)
        {
                printf("Error (%d) opening NVS handle!\n", err);
        }
        else
        {
                printf("ratio_state Done\n");
                // Write
                printf("Updating restart counter in NVS ... ");
                err = nvs_set_blob(my_handle, "ratio_state", &ptr, sizeof(ratio_val));
                printf((err != ESP_OK) ? "Failed!\n" : "ratio_state Done\n");


                printf("Committing updates in NVS ... ");
                err = nvs_commit(my_handle);
                printf((err != ESP_OK) ? "Failed!\n" : "ratio_state Done\n");

                // Close
                nvs_close(my_handle);
        }
}

bool read_ratio(ratio_val *ptr)
{
        // Open
        printf("\n");
        printf("Opening Non-Volatile Storage (NVS) handle...\n");
        nvs_handle my_handle;
        esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
        if (err != ESP_OK)
        {
                printf("Error (%d) opening NVS handle!\n", err);
                return false;
        }
        else
        {
                printf("ratio_state Done\n");

                // Read
                printf("R  อeading restart counter from NVS ...\n");
                size_t size_read = 0;
                err = nvs_get_blob(my_handle, "ratio_state", NULL, &size_read);
                if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
                        return err;

                printf("size :%d\n", size_read);

                err = nvs_get_blob(my_handle, "ratio_state", ptr, &size_read);
                switch (err)
                {
                case ESP_OK:
                        printf("Done\n");
                        printf("Restart counter = %d\n", size_read);

                        break;
                case ESP_ERR_NVS_NOT_FOUND:
                        printf("The value is not initialized yet!\n");
                        return false;
                        break;
                default:
                        printf("Error (%d) reading!\n", err);
                        return false;
                        break;
                }

                nvs_close(my_handle);
                return true;
        }
}


void save_ph_kvalue(ph_str_val ptr)
{
        // Open
        printf("\n");
        printf("Opening Non-Volatile Storage (NVS) handle...\n");
        nvs_handle my_handle;
        esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
        if (err != ESP_OK)
        {
                printf("Error (%d) opening NVS handle!\n", err);
        }
        else
        {
                printf("ph_state Done\n");
                // Write
                printf("Updating restart counter in NVS ... ");
                err = nvs_set_blob(my_handle, "ph_state", &ptr, sizeof(ph_str_val));
                printf((err != ESP_OK) ? "Failed!\n" : "ph_state Done\n");


                printf("Committing updates in NVS ... ");
                err = nvs_commit(my_handle);
                printf((err != ESP_OK) ? "Failed!\n" : "ph_state Done\n");

                // Close
                nvs_close(my_handle);
        }
}

bool read_ph_kvalue(ph_str_val *ptr)
{
        // Open
        printf("\n");
        printf("Opening Non-Volatile Storage (NVS) handle...\n");
        nvs_handle my_handle;
        esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
        if (err != ESP_OK)
        {
                printf("Error (%d) opening NVS handle!\n", err);
                return false;
        }
        else
        {
                printf("ph_state Done\n");

                // Read
                printf("Reading restart counter from NVS ...\n");
                size_t size_read = 0;
                err = nvs_get_blob(my_handle, "ph_state", NULL, &size_read);
                if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
                        return err;

                printf("size :%d\n", size_read);

                err = nvs_get_blob(my_handle, "ph_state", ptr, &size_read);
                switch (err)
                {
                case ESP_OK:
                        printf("Done\n");
                        printf("Restart counter = %d\n", size_read);

                        break;
                case ESP_ERR_NVS_NOT_FOUND:
                        printf("The value is not initialized yet!\n");
                        return false;
                        break;
                default:
                        printf("Error (%d) reading!\n", err);
                        return false;
                        break;
                }


                nvs_close(my_handle);
                return true;
        }
}
